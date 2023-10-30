#pragma once

#include "IMSPFunctions.h"
#ifdef __linux__

#include <dlfcn.h>
#include <stdint.h>
#include <iostream>
class MSPFunctions :
	public IMSPFunctions
{
public:
	MSPFunctions();
	~MSPFunctions();

	virtual uint8_t* Get_Kek_By_Info(int dataLength, int* data, int crc);
	virtual uint8_t* Encrypt_data_key(int dataLength, int* data, int crc);
	virtual uint8_t* Decrypt_data_key(int dataLength, int* data, int crc);

};
#endif // __linux__