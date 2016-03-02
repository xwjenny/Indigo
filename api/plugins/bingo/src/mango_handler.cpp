#include "mango_handler.h"
#include "bingo_core_c.h"
#include "bingo_mapping_obj.h"
#include "indigo_mapping.h"
#include "indigo_molecule.h"

#include "base_cpp/scanner.h"
#include "base_cpp/output.h"
#include "molecule/icm_saver.h"
#include "molecule/icm_loader.h"

void MangoHandler::setupMatch(const char *search_type, const char *query, const char *options)
{
	Indigo &self = indigoGetInstance();
	int query_id = indigoLoadQueryMoleculeFromString(query);

	_query = &((IndigoQueryMolecule &)self.getObject(query_id)).getQueryMolecule();

	mangoSetupMatch(search_type, query, options);
}

IndigoObject * MangoHandler::matchStructure(IndigoObject * index_data)
{
	BingoIndexData * data = (BingoIndexData *)index_data;
	
	indigo::Array<byte> & icm = data->getIcm();
	int res = mangoMatchTarget((const char *)icm.ptr(), icm.size());

	int target = _loadIcmMolecule(icm.ptr(), icm.size());

	int *mapping;
	int mapping_len;
	mangoGetMapping(&mapping, &mapping_len);

	AutoPtr<BingoMoleculeMappingObj> mapping_obj(new BingoMoleculeMappingObj(res == 1));

	mapping_obj->mapping.copy(mapping, mapping_len);

	return mapping_obj.release();
}

bool MangoHandler::_readPreparedInfo(int* id, int fp_size)
{
	int bingo_res;
	const char* cmf_buf;
	int cmf_len;
	const char*xyz_buf;
	int xyz_len;
	const char*gross_str;
	const char*counter_elements_str;
	const char*fp_buf;
	int fp_len;
	const char *fp_sim_str;
	float mass;
	int sim_fp_bits_count;
	/*
	* Get prepared data
	*/
	bingo_res = mangoIndexReadPreparedMolecule(id, &cmf_buf, &cmf_len, &xyz_buf, &xyz_len,
		&gross_str, &counter_elements_str, &fp_buf, &fp_len,
		&fp_sim_str, &mass, &sim_fp_bits_count);

	BingoIndexData &index_data = _index_data.push();

	Array<char> icm;
	ArrayOutput array_output(icm);
	IcmSaver icm_saver(array_output);

   if ((xyz_buf != 0) && (xyz_len > 0))
	   icm_saver.save_xyz = true;

	icm_saver.save_bond_dirs = true;

	icm_saver.saveMolecule(cmf_buf, cmf_len, xyz_buf, xyz_len);

	index_data.setIcm((const byte *)icm.ptr(), icm.size());
	index_data.setFingerprint((const byte *)fp_buf, fp_len);
	index_data.setGrossStr(gross_str);
	index_data.setCounterElementsStr(counter_elements_str);
	index_data.setFpSimStr(fp_sim_str);
	index_data.setMass(mass);
	index_data.setSimFpBitsCount(sim_fp_bits_count);

	return true;
}

int MangoHandler::_loadIcmMolecule (const byte *buf, int buf_len)
{
	Indigo &self = indigoGetInstance();

	AutoPtr<IndigoMolecule> target_obj(new IndigoMolecule());
	BufferScanner scanner(buf, buf_len);
	IcmLoader loader(scanner);
	loader.loadMolecule(target_obj->getMolecule());
	return self.addObject(target_obj.release());
}