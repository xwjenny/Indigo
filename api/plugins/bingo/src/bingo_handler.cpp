#include "bingo_handler.h"
#include "bingo_core_c.h"

#include "base_cpp/tlscont.h"
#include "base_cpp/array.h"

using namespace indigo;

BingoHandler::BingoHandler()
{
	_bingoSession = bingoAllocateSessionID();
}

BingoHandler::~BingoHandler()
{
	bingoReleaseSessionID(_bingoSession);
}

void BingoHandler::_setBingoContext() 
{
	bingoSetSessionID(_bingoSession);
	bingoSetContext(0);
}

int BingoHandler::_getNextRecordCb(void *context) 
{
	BingoHandler* handler = (BingoHandler*)context;

	return 1;
}

void BingoHandler::_processErrorCb(int id, void *context) 
{
	BingoHandler* handler = (BingoHandler*)context;
}