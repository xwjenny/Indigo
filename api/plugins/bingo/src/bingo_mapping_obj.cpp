#include "bingo_mapping_obj.h"

BingoMappingObj::BingoMappingObj(bool is_matched) : IndigoObject(BINGO_MAPPING), _is_matched(is_matched)
{
}

BingoMappingObj::~BingoMappingObj()
{
}

bool BingoMappingObj::isMatched()
{
	return _is_matched;
}

BingoMoleculeMappingObj::BingoMoleculeMappingObj(bool is_matched) : BingoMappingObj(is_matched)
{
	mapping.clear();
}

BingoMoleculeMappingObj::~BingoMoleculeMappingObj()
{
	mapping.clear();
}

BingoMoleculeMappingObj * BingoMoleculeMappingObj::cloneFrom(IndigoObject & obj)
{
	BingoMoleculeMappingObj & other = (BingoMoleculeMappingObj &)obj;

	AutoPtr<BingoMoleculeMappingObj> mapping_ptr(new BingoMoleculeMappingObj(other._is_matched));

	mapping_ptr->mapping.copy(other.mapping);

	return mapping_ptr.release();
}

IndigoObject * BingoMoleculeMappingObj::clone()
{
	return cloneFrom(*this);
}

BingoReactionMappingObj::BingoReactionMappingObj(bool is_matched) : BingoMappingObj(is_matched)
{
	mol_mapping.clear();
	mappings.clear();
}

BingoReactionMappingObj::~BingoReactionMappingObj()
{
	mol_mapping.clear();
	mappings.clear();
}

BingoReactionMappingObj * BingoReactionMappingObj::cloneFrom(IndigoObject & obj)
{
	BingoReactionMappingObj & other = (BingoReactionMappingObj &)obj;

	AutoPtr<BingoReactionMappingObj> mapping_ptr(new BingoReactionMappingObj(other._is_matched));

	mapping_ptr->mol_mapping.copy(other.mol_mapping);

	for (int i = 0; i < other.mappings.size(); i++)
	{
		Array<int> &reactant_mapping = mapping_ptr->mappings.push();

		reactant_mapping.copy(other.mappings[i]);
	}

	return mapping_ptr.release();
}

IndigoObject * BingoReactionMappingObj::clone()
{
	return cloneFrom(*this);
}
