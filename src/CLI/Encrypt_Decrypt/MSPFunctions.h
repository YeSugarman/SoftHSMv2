#pragma once

#include "IMSPFunctions.h"
#ifdef __linux__

#include <dlfcn.h>
#include <stdint.h>
#include <iostream>
class MSPFunctions :
	public IMSPFunctions
{
private:
  void* handle;
public:
	MSPFunctions();
	~MSPFunctions();

	virtual uint8_t* Get_Kek_By_Info(long userId, int info_len, std::string kekInfo) override;

	virtual uint8_t* Encrypt_data_key(long userId, uint8_t kekId, uint8_t key_length, uint8_t* arrRandom) override;

	virtual uint8_t* Decrypt_data_key(long userId, uint8_t kekId, uint8_t encryptedKeyLength, uint8_t* encryptedKey) override;

};
#endif // __linux__