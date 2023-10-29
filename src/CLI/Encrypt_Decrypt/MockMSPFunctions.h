#pragma once
#include "IMSPFunctions.h"

class MockMSPFunctions :
	public IMSPFunctions
{
public:
	MockMSPFunctions();
	~MockMSPFunctions();

	virtual uint8_t* Get_Kek_By_Info(int dataLenght, int* data, int crc);
	virtual uint8_t* Encrypt_data_key(int dataLenght, int* data, int crc);
	virtual uint8_t* Decrypt_data_key(int dataLenght, int* data, int crc);

	//virtual uint8_t* read(int magic, int command, int dataLenght, int* data, int crc);
	//virtual void write(int* magic, int* status, int* dataLenght, int* result, int* crc);
};