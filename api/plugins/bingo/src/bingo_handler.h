#ifndef _BINGO_HANDLER_H__
#define _BINGO_HANDLER_H__

/*
* Interface class for procession fingerprint data
*/

#include "base_cpp/auto_ptr.h"
#include "base_cpp/obj_array.h"

class BingoHandler {
public:
	BingoHandler();
	virtual ~BingoHandler();
private:
	BingoHandler(const BingoHandler&); //no implicit copy
protected:
	void _setBingoContext();

	static int _getNextRecordCb(void *context);
	static void _processErrorCb(int id, void *context);

	qword _bingoSession;
};



#endif /* BINGO_HANDLER_H */

