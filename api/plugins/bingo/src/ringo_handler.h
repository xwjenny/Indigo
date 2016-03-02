#ifndef RINGO_HANDLER_H
#define	RINGO_HANDLER_H

#include "base_cpp/array.h"
#include "base_cpp/auto_ptr.h"
#include "base_cpp/exception.h"

#include "bingo_handler.h"

/*
* Class for procession reaction fingerprint data
*/
class RingoHandler : public BingoHandler {
public:
	RingoHandler() : BingoHandler(true)
	{
		_query = NULL;
	}

	virtual void setupMatch(const char *search_type, const char *query, const char *options);

	virtual IndigoObject * matchStructure(IndigoObject * index_data);

private:
	QueryReaction *_query;

	virtual bool _readPreparedInfo(int* id, int fp_size);

	int _loadCrfReaction(const byte *buf, int buf_len);
};

#endif	/* RINGO_HANDLER_H */