#pragma once
#include "IMSPFunctions.h"

class MockMSPFunctions :
	public IMSPFunctions
{
public:
	MockMSPFunctions();
	~MockMSPFunctions();

	virtual void read(int magic, int command, int dataLenght, int* data, int crc);

	virtual void write(int* magic, int* status, int* dataLenght, int* result, int* crc);

};