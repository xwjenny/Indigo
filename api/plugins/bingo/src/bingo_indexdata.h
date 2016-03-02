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

#ifndef __indigo_indexdata__
#define __indigo_indexdata__

#include "indigo_internal.h"
#include "molecule/query_molecule.h"

class BingoIndexData : public IndigoObject
{
public:
	BingoIndexData();
	virtual ~BingoIndexData();

   void setIcm(Array<byte> &icm);
   void setIcm(const byte* icm_buf, int icm_len);
   
   void setFingerprint(Array<byte> &fp);
   void setFingerprint(const byte* fp_buf, int fp_len);
   
   void setGrossStr(Array<char> &gross_str);
   void setGrossStr(const char* gross_str);
   
   void setCounterElementsStr(Array<char> &counter_elements_str);
   void setCounterElementsStr(const char* counter_elements_str);
   
   void setFpSimStr(Array<char> &fp_sim_str);
   void setFpSimStr(const char* fp_sim_str);
   
   void setMass(float mass);
   void setSimFpBitsCount(int sim_fp_bits_count);

   Array<byte> & getIcm();
   Array<byte> & getFingerprint();
   Array<char> & getGrossStr();
   Array<char> & getCounterElementsStr();
   Array<char> & getFpSimStr();
   float getMass();
   int getSimFpBitsCount();

   static BingoIndexData * cloneFrom(IndigoObject & obj);
   virtual IndigoObject * clone();

private:
	Array<byte> _icm;
	Array<byte> _fp;
	Array<char> _gross_str;
	Array<char> _counter_elements_str;
	Array<char> _fp_sim_str;

	float _mass;
	int _sim_fp_bits_count;
};

class BingoBuffer : public IndigoObject
{
public:
	BingoBuffer();
	BingoBuffer(Array<byte> &buf);
	BingoBuffer(const byte *buf, int size);
	BingoBuffer(const char *buf);
	virtual ~BingoBuffer();

	void setBuf(Array<byte> &buf);

	Array<byte> & getBuf();

	static BingoBuffer * cloneFrom(IndigoObject & obj);
	virtual IndigoObject * clone();

private:
	Array<byte> _buf;
};

#endif