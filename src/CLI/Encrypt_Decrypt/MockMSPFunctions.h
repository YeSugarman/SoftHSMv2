#pragma once
#include "IMSPFunctions.h"

class MockMSPFunctions :
	public IMSPFunctions
{
public:
	MockMSPFunctions();
	~MockMSPFunctions();

	virtual uint8_t* Get_Kek_By_Info(long userId, std::string kekInfo) override;

	virtual uint8_t* Encrypt_data_key(long userId, uint8_t kekId, uint8_t key_length, uint8_t* arrRandom) override;
									  	  
	virtual uint8_t* Decrypt_data_key(long userId, uint8_t kekId, uint8_t encryptedKeyLength, uint8_t* encryptedKey) override;

	//virtual uint8_t* read(int magic, int command, int dataLength, int* data, int crc);
	//virtual void write(int* magic, int* status, int* dataLength, int* result, int* crc);
};