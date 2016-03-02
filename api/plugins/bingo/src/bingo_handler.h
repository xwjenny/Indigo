#ifndef _BINGO_HANDLER_H__
#define _BINGO_HANDLER_H__

/*
* Interface class for procession fingerprint data
*/

#include "indigo_internal.h"
#include "indigo_mapping.h"
#include "bingo_indexdata.h"

#include "base_cpp/auto_ptr.h"
#include "base_cpp/obj_array.h"
#include "base_cpp/ptr_array.h"

class BingoHandler {
public:
	BingoHandler (bool is_reaction);
	virtual ~BingoHandler();

	int getFpSize();

	indigo::ObjArray<BingoIndexData> & indexStructures(indigo::PtrArray<IndigoObject>& objects);

	virtual void setupMatch(const char *search_type, const char *query, const char *options) = 0;

	virtual IndigoObject * matchStructure(IndigoObject * index_data) = 0;

private:
	BingoHandler(const BingoHandler&); //no implicit copy

	int _cache_idx;
	int _fp_size;

	indigo::ObjArray<Array<char>> _struct_caches;

protected:
	void _initBingo();

	void _setBingoContext();

	virtual bool _readPreparedInfo(int* id, int fp_size) = 0;

	static int _getNextRecordCb(void *context);
	static void _processErrorCb(int id, void *context);
	static void _processResultCb(void *context);

	indigo::ObjArray<BingoIndexData> _index_data;
	indigo::Array<bool> _output_results;

	qword _bingoSession;
	bool _is_reaction;
};



#endif /* BINGO_HANDLER_H */

