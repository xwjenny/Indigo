#include "bingo_pg_buffer.h"
#include "base_cpp/array.h"
#include "base_cpp/tlscont.h"

CEXPORT {
#include "postgres.h"
#include "fmgr.h"
#include "storage/bufmgr.h"
#include "utils/rel.h"
#include "storage/bufmgr.h"
#include "access/itup.h"
#include "utils/relcache.h"
#include "storage/lock.h"
}
/*
 * Empty buffer constructor
 */
BingoPgBuffer::BingoPgBuffer():
_buffer(InvalidBuffer),
_lock(BINGO_PG_NOLOCK),
_blockIdx(0){
}
/*
 * New buffer constructor
 */
BingoPgBuffer::BingoPgBuffer(PG_OBJECT rel_ptr, unsigned int block_num) {
   writeNewBuffer(rel_ptr, block_num);
}
/*
 * Existing buffer constructor
 */
BingoPgBuffer::BingoPgBuffer(PG_OBJECT rel_ptr, unsigned int block_num, int lock) {
   readBuffer(rel_ptr, block_num, lock);
}
/*
 * Destructor
 */
BingoPgBuffer::~BingoPgBuffer() {
   clear();
}
/*
 * Changes an access for the buffer
 */
void BingoPgBuffer::changeAccess(int lock) {
   if(_buffer == InvalidBuffer)
      return;
   if (_lock == BINGO_PG_WRITE)
      MarkBufferDirty(_buffer);
   if (_lock != BINGO_PG_NOLOCK)
      LockBuffer(_buffer, BUFFER_LOCK_UNLOCK);
   if (lock != BINGO_PG_NOLOCK)
      LockBuffer(_buffer, _getAccess(lock));
   _lock = lock;
}
 /*
 * Writes a new buffer with WRITE lock
 */
int BingoPgBuffer::writeNewBuffer(PG_OBJECT rel_ptr, unsigned int block_num) {
   /*
    * Clear if it is a new buffer
    */
   if(_buffer != InvalidBuffer) {
      if(_blockIdx != block_num)
         clear();
      else
         return _buffer;
   }
   Relation rel = (Relation) rel_ptr;
   BlockNumber nblocks = RelationGetNumberOfBlocks(rel);

   /*
    * Bingo forbids noncontiguous access
    */
   if (block_num > nblocks)
      elog(ERROR, "internal error: access to noncontiguous page in bingo index \"%s\"",
           RelationGetRelationName(rel));
//   if(block_num < nblocks)
//      elog(ERROR, "internal error: access to already pinned block in bingo index");

   /*
    * smgr insists we use P_NEW to extend the relation
    */
   if (block_num == nblocks) {
      _buffer = ReadBuffer(rel, P_NEW);
      if (BufferGetBlockNumber(_buffer) != block_num)
         elog(ERROR, "unexpected relation size: %u, should be %u",
              BufferGetBlockNumber(_buffer), block_num);
   } else
      _buffer = ReadBufferExtended(rel, MAIN_FORKNUM,  block_num, RBM_ZERO, NULL);
   /*
    * Lock buffer on writing
    */
   LockBuffer(_buffer, BUFFER_LOCK_EXCLUSIVE);

   /* 
    * initialize the page
    */
//   PageInit(BufferGetPage(buf), BufferGetPageSize(buf), sizeof (HashPageOpaqueData));
   PageInit(BufferGetPage(_buffer), BufferGetPageSize(buf), 0);
   _lock = BINGO_PG_WRITE;
    /*
     * Store block index
     */
   _blockIdx = block_num;
   return _buffer;
}
/*
 * Reads a buffer
 */
int BingoPgBuffer::readBuffer(PG_OBJECT rel_ptr, unsigned int block_num, int lock) {
   /*
    * Clear a buffer if it is different
    */
   if(_buffer != InvalidBuffer) {
      if(_blockIdx != block_num)
         clear();
      else {
         changeAccess(lock);
         return _buffer;
      }
   }

   Relation rel = (Relation) rel_ptr;
   Buffer buf = ReadBuffer(rel, block_num);

   /*
    * Lock buffer
    */
   if (lock != BINGO_PG_NOLOCK)
      LockBuffer(buf, _getAccess(lock));

   _lock = lock;
   _buffer = buf;
   /*
    * Store block index
    */
   _blockIdx = block_num;
   return _buffer;
}

/*
 * Clears and releases the buffer
 */
void BingoPgBuffer::clear() {
   if(_buffer == InvalidBuffer)
      return;
   switch(_lock) {
      case BINGO_PG_WRITE:
         MarkBufferDirty(_buffer);
         UnlockReleaseBuffer(_buffer);
         break;
      case BINGO_PG_READ:
         UnlockReleaseBuffer(_buffer);
         break;
      case BINGO_PG_NOLOCK:
         ReleaseBuffer(_buffer);
         break;
   }
   _buffer = InvalidBuffer;
   _lock = BINGO_PG_NOLOCK;
   _blockIdx = 0;
}

int BingoPgBuffer::_getAccess(int lock) {
   switch(lock) {
      case BINGO_PG_WRITE:
         return BUFFER_LOCK_SHARE;
      case BINGO_PG_READ:
         return BUFFER_LOCK_EXCLUSIVE;
   }
   return BUFFER_LOCK_UNLOCK;
}

void* BingoPgBuffer::getIndexData(int& data_len) {
   Page page = BufferGetPage(getBuffer());
   IndexTuple itup = (IndexTuple) PageGetItem(page, PageGetItemId(page, BINGO_TUPLE_OFFSET));

   int hoff = IndexInfoFindDataOffset(itup->t_info);
   char* data_ptr = (char *) itup + hoff;

   data_len = IndexTupleSize(itup) - hoff;

   if(data_ptr == 0)
      elog(ERROR, "internal error: empty ptr data for the block %d", _blockIdx);

   return data_ptr;
}

void BingoPgBuffer::formIndexTuple(void* map_data, int size) {
   Page page = BufferGetPage(getBuffer());
   Datum map_datum = PointerGetDatum(map_data);

   TupleDesc index_desc = CreateTemplateTupleDesc(1, false);
   index_desc->attrs[0]->attlen = size;
   index_desc->attrs[0]->attalign = 'c';
   index_desc->attrs[0]->attbyval = false;
   bool isnull = false;

   IndexTuple itup = index_form_tuple(index_desc, &map_datum, &isnull);
   int itemsz = IndexTupleDSize(*itup);
   itemsz = MAXALIGN(itemsz);

   if (PageAddItem(page, (Item) itup, itemsz, 0, false, false) == InvalidOffsetNumber) {
      pfree(itup);
      FreeTupleDesc(index_desc);
      elog(ERROR, "failed to add index item");
   }

   pfree(itup);
   FreeTupleDesc(index_desc);
}

using namespace indigo;

void BingoPgBuffer::formEmptyIndexTuple(int size) {
   QS_DEF(Array<char>, buf);
   buf.resize(size);
   buf.zerofill();
   formIndexTuple(buf.ptr(), buf.sizeInBytes());
}

bool BingoPgBuffer::isReady() const {
   return _buffer != InvalidBuffer;
}