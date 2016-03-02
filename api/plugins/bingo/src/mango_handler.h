#ifndef MANGO_HANDLER_H
#define	MANGO_HANDLER_H

#include "base_cpp/array.h"
#include "base_cpp/auto_ptr.h"
#include "base_cpp/exception.h"

#include "bingo_handler.h"

class BingoPgText;
class BingoPgIndex;
class BingoPgConfig;
class BingoPgFpData;
class MangoPgFpData;

/*
* Class for procession molecule fingerprint data
*/
class MangoHandler : public BingoHandler {
public:
	MangoHandler() : BingoHandler(false)
	{
		_query = NULL;
	}

	virtual void setupMatch(const char *search_type, const char *query, const char *options);

	virtual IndigoObject * matchStructure(IndigoObject * index_data);

private:
	QueryMolecule *_query;

	virtual bool _readPreparedInfo(int* id, int fp_size);

	int _loadIcmMolecule(const byte *buf, int buf_len);
};

#endif	/* MANGO_HANDLER_H */
