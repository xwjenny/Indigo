/****************************************************************************
 * Copyright (C) 2009-2015 EPAM Systems
 *
 * This file is part of Indigo toolkit.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "bingo_indexdata.h"

BingoIndexData::BingoIndexData() : IndigoObject(BINGO_INDEXDATA)
{
	_fp.clear();
	_icm.clear();
	_gross_str.clear();
	_counter_elements_str.clear();
	_fp_sim_str.clear();

	_mass = 0;
	_sim_fp_bits_count = 0;
}

BingoIndexData::~BingoIndexData()
{
	_fp.clear();
	_icm.clear();
	_gross_str.clear();
	_counter_elements_str.clear();
	_fp_sim_str.clear();

	_mass = 0;
	_sim_fp_bits_count = 0;
}


void BingoIndexData::setIcm(Array<byte> &icm)
{
	_icm.clear();
	_icm.copy(icm);
}

void BingoIndexData::setIcm(const byte* icm_buf, int icm_len)
{
	_icm.clear();
	_icm.copy(icm_buf, icm_len);
}

void BingoIndexData::setFingerprint(Array<byte> &fp)
{
	_fp.clear();
	_fp.copy(fp);
}

void BingoIndexData::setFingerprint(const byte* fp_buf, int fp_len)
{
	_fp.clear();
	_fp.copy(fp_buf, fp_len);
}

void BingoIndexData::setGrossStr(Array<char> &gross_str)
{
	_gross_str.clear();
	_gross_str.copy(gross_str);
}

void BingoIndexData::setGrossStr(const char* gross_str)
{
	_gross_str.clear();
	_gross_str.appendString(gross_str, true);
}

void BingoIndexData::setCounterElementsStr(Array<char> &counter_elements_str)
{
	_counter_elements_str.clear();
	_counter_elements_str.copy(counter_elements_str);
}

void BingoIndexData::setCounterElementsStr(const char* counter_elements_str)
{
	_counter_elements_str.clear();
	_counter_elements_str.appendString(counter_elements_str, true);
}


void BingoIndexData::setFpSimStr(Array<char> &fp_sim_str)
{
	_fp_sim_str.clear();
	_fp_sim_str.copy(fp_sim_str);
}

void BingoIndexData::setFpSimStr(const char* fp_sim_str)
{
	_fp_sim_str.clear();
	_fp_sim_str.appendString(fp_sim_str, true);
}

void BingoIndexData::setMass(float mass)
{
	_mass = mass;
}


void BingoIndexData::setSimFpBitsCount(int sim_fp_bits_count)
{
	_sim_fp_bits_count = sim_fp_bits_count;
}

Array<byte> & BingoIndexData::getIcm()
{
	return _icm;
}

Array<byte> & BingoIndexData::getFingerprint()
{
	return _fp;
}

Array<char> & BingoIndexData::getGrossStr()
{
	return _gross_str;
}

Array<char> & BingoIndexData::getCounterElementsStr()
{
	return _counter_elements_str;
}

Array<char> & BingoIndexData::getFpSimStr()
{
	return _fp_sim_str;
}

float BingoIndexData::getMass()
{
	return _mass;
}

int BingoIndexData::getSimFpBitsCount()
{
	return _sim_fp_bits_count;
}




BingoIndexData * BingoIndexData::cloneFrom(IndigoObject & obj)
{
	BingoIndexData & other_data = (BingoIndexData &)obj;

	AutoPtr<BingoIndexData> data_ptr(new BingoIndexData());

	data_ptr->setIcm(other_data.getIcm());
	data_ptr->setFingerprint(other_data.getFingerprint());
	data_ptr->setGrossStr(other_data.getGrossStr());
	data_ptr->setCounterElementsStr(other_data.getCounterElementsStr());
	data_ptr->setFpSimStr(other_data.getFpSimStr());
	data_ptr->setMass(other_data.getMass());
	data_ptr->setSimFpBitsCount(other_data.getSimFpBitsCount());

	return data_ptr.release();
}

IndigoObject * BingoIndexData::clone()
{
	return cloneFrom(*this);
}


BingoBuffer::BingoBuffer() : IndigoObject(BINGO_INDEXDATA)
{
	_buf.clear();
}

BingoBuffer::BingoBuffer(Array<byte> &buf) : IndigoObject(BINGO_INDEXDATA)
{
	_buf.copy(buf);
}

BingoBuffer::BingoBuffer(const byte *buf, int size) : IndigoObject(BINGO_INDEXDATA)
{
	_buf.copy(buf, size);
}

BingoBuffer::BingoBuffer(const char *buf) : IndigoObject(BINGO_INDEXDATA)
{
	_buf.clear();
	_buf.appendString(buf, 0);
}

BingoBuffer::~BingoBuffer()
{
	_buf.clear();
}

void BingoBuffer::setBuf(Array<byte> &buf)
{
	_buf.copy(buf);
}

Array<byte> & BingoBuffer::getBuf()
{
	return _buf;
}

BingoBuffer * BingoBuffer::cloneFrom(IndigoObject & obj)
{
	BingoBuffer & other_data = (BingoBuffer &)obj;

	AutoPtr<BingoBuffer> data_ptr(new BingoBuffer());

	data_ptr->setBuf(other_data.getBuf());

	return data_ptr.release();
}

IndigoObject * BingoBuffer::clone()
{
	return cloneFrom(*this);
}