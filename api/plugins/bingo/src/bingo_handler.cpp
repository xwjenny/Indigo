#include "bingo_handler.h"
#include "bingo_core_c.h"

#include "base_cpp/tlscont.h"
#include "base_cpp/array.h"
#include "base_cpp/output.h"
#include "base_cpp/scanner.h"

#include "reaction/base_reaction.h"
#include "reaction/icr_saver.h"
#include "reaction/crf_saver.h"
#include "reaction/rsmiles_saver.h"

#include "molecule/base_molecule.h"
#include "molecule/icm_saver.h"
#include "molecule/cmf_saver.h"
#include "molecule/smiles_saver.h"

#include "indigo_molecule.h"
#include "indigo_reaction.h"
#include "indigo_io.h"

using namespace indigo;

BingoHandler::BingoHandler(bool is_reaction) : _is_reaction(is_reaction)
{
	_bingoSession = bingoAllocateSessionID();
	_cache_idx = 0;

	_initBingo();
}

BingoHandler::~BingoHandler()
{
	bingoReleaseSessionID(_bingoSession);
}

indigo::ObjArray<BingoIndexData> & BingoHandler::indexStructures(indigo::PtrArray<IndigoObject>& structures)
{
	for (int i = 0; i < structures.size(); i++)
	{
		Array<char> &str = _struct_caches.push();
		BingoBuffer *bingo_buffer = (BingoBuffer *)(structures.at(i));

		Array<byte> &buf = bingo_buffer->getBuf();

		str.copy((char *)buf.ptr(), buf.size());
	}

	int begin_res = bingoIndexBegin();
	int bingo_res = bingoIndexProcess(_is_reaction, _getNextRecordCb, _processResultCb, _processErrorCb, this);
	int end_res = bingoIndexEnd();

	return _index_data;
}

int BingoHandler::getFpSize() 
{
	int result;

	bingoGetConfigInt("fp-size-bytes", &result);

	return result * 8;
}

void BingoHandler::_initBingo() 
{
	_setBingoContext();

	bingoSetConfigInt("treat_x_as_pseudoatom", 0);
	bingoSetConfigInt("ignore_closing_bond_direction_mismatch", 0);

	bingoSetConfigInt("ignore_cistrans_errors", 0);
	bingoSetConfigInt("ignore_stereocenter_errors", 0);
	
	bingoSetConfigInt("allow_non_unique_dearomatization", 0);
	bingoSetConfigInt("zero_unknown_aromatic_hydrogens", 0);

	bingoSetConfigInt("stereochemistry_bidirectional_mode", 0);
	bingoSetConfigInt("stereochemistry_detect_haworth_projection", 0);
	
	bingoSetConfigInt("reject_invalid_structures", 0);

	bingoSetConfigInt("no_cmf_vocabulary", 1);

	bingoSetConfigInt("FP_ORD_SIZE", 25);
	bingoSetConfigInt("FP_ANY_SIZE", 15);
	bingoSetConfigInt("FP_TAU_SIZE", 10);
	bingoSetConfigInt("FP_SIM_SIZE", 8);

	bingoSetConfigInt("SUB_SCREENING_MAX_BITS", 8);
	bingoSetConfigInt("SIM_SCREENING_PASS_MARK", 128);

	bingoSetConfigInt("NTHREADS", -1);
	bingoSetConfigInt("TIMEOUT", 60000);

	_fp_size = getFpSize();
}

void BingoHandler::_setBingoContext() 
{
	bingoSetSessionID(_bingoSession);
	bingoSetContext(0);
}

int BingoHandler::_getNextRecordCb(void *context) 
{
	BingoHandler* handler = (BingoHandler*)context;

	if (handler->_cache_idx >= handler->_struct_caches.size())
		return 0;

	Array<char> &str = handler->_struct_caches.at(handler->_cache_idx);

	int set_res = bingoSetIndexRecordData(handler->_cache_idx, str.ptr(), str.size());
	
	handler->_cache_idx++;

	return 1;
}

void BingoHandler::_processErrorCb(int id, void *context) 
{
	BingoHandler* handler = (BingoHandler*)context;

	printf("%s", bingoGetError());
	printf("%s", bingoGetWarning());
}

void BingoHandler::_processResultCb(void *context)
{
	BingoHandler* handler = (BingoHandler*)context;

	int id;

	handler->_readPreparedInfo(&id, handler->_fp_size);
}