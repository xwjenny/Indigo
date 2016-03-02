#ifndef __bingo_mapping_obj__
#define __bingo_mapping_obj__

#include "indigo_internal.h"

class BingoMappingObj : public IndigoObject
{
public:
	BingoMappingObj(bool is_matched);
	virtual ~BingoMappingObj();

	virtual IndigoObject * clone() = 0;

	bool isMatched();

protected:
	bool _is_matched;
};

class BingoMoleculeMappingObj : public BingoMappingObj
{
public:
	BingoMoleculeMappingObj(bool is_matched);
	virtual ~BingoMoleculeMappingObj();

	Array<int> mapping;

	static BingoMoleculeMappingObj * cloneFrom(IndigoObject & obj);

	virtual IndigoObject * clone();
};

class BingoReactionMappingObj : public BingoMappingObj
{
public:
	BingoReactionMappingObj(bool is_matched);
	virtual ~BingoReactionMappingObj();

	Array< int > mol_mapping;
	ObjArray< Array<int> > mappings;

	static BingoReactionMappingObj * cloneFrom(IndigoObject & obj);

	virtual IndigoObject * clone();
};

#endif
