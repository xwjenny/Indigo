#include "ringo_handler.h"
#include "bingo_core_c.h"
#include "bingo_mapping_obj.h"

#include "indigo_reaction.h"
#include "base_cpp/scanner.h"
#include "base_cpp/output.h"
#include "reaction/icr_loader.h"
#include "reaction/icr_saver.h"

void RingoHandler::setupMatch(const char *search_type, const char *query, const char *options)
{
	Indigo &self = indigoGetInstance();
	int query_id = indigoLoadQueryReactionFromString(query);

	_query = &((IndigoQueryReaction &)self.getObject(query_id)).getQueryReaction();

	ringoSetupMatch(search_type, query, options);
}

IndigoObject * RingoHandler::matchStructure(IndigoObject * index_data)
{
	BingoIndexData * data = (BingoIndexData *)index_data;

	indigo::Array<byte> & icr = data->getIcm();

	int res = ringoMatchTargetBinary((const char *)icr.ptr(), icr.size());

	int target = _loadCrfReaction(icr.ptr(), icr.size());

	int *mol_mapping;
	int mol_mapping_len;
	ringoGetMolMapping(&mol_mapping, &mol_mapping_len);

	AutoPtr<BingoReactionMappingObj> mapping_obj(new BingoReactionMappingObj(res == 1));

	if (res != 1)
		return mapping_obj.release();

	mapping_obj->mol_mapping.copy(mol_mapping, mol_mapping_len);

	mapping_obj->mappings.clear();
	mapping_obj->mappings.resize(_query->end());

	for (int i = _query->begin(); i != _query->end(); i = _query->next(i))
	{
		int *mapping;
		int mapping_len;
		ringoGetMapping(i, &mapping, &mapping_len);

		mapping_obj->mappings[i].copy(mapping, mapping_len);
	}

	return mapping_obj.release();
}

bool RingoHandler::_readPreparedInfo(int* id, int fp_size)
{
	int bingo_res;
	const char* crf_buf;
	int crf_len;
	const char*fp_buf;
	int fp_len;

	/*
	* Get prepared data
	*/
	bingo_res = ringoIndexReadPreparedReaction(id, &crf_buf, &crf_len, &fp_buf, &fp_len);

	BingoIndexData &index_data = _index_data.push();

	Array<char> icr;
	ArrayOutput array_output(icr);
	IcrSaver icr_saver(array_output);

	icr_saver.save_xyz = true;
	icr_saver.save_bond_dirs = true;

	icr_saver.saveReaction(crf_buf, crf_len);

	index_data.setIcm((const byte *)icr.ptr(), icr.size());
	index_data.setFingerprint((const byte *)fp_buf, fp_len);

	return true;
}


int RingoHandler::_loadCrfReaction(const byte *buf, int buf_len)
{
	Indigo &self = indigoGetInstance();

	AutoPtr<IndigoReaction> target_obj(new IndigoReaction());
	BufferScanner scanner(buf, buf_len);
	IcrLoader loader(scanner);
	loader.loadReaction(target_obj->getReaction());
	return self.addObject(target_obj.release());
}