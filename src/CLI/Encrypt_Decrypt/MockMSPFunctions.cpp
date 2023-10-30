#include "MockMSPFunctions.h"

MockMSPFunctions::MockMSPFunctions()
{
}

MockMSPFunctions::~MockMSPFunctions()
{
}

uint8_t* MockMSPFunctions::Get_Kek_By_Info(long userId, std::string kekInfo)
{
	uint8_t* writeArr = new uint8_t[5];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = 1;//data Length
	writeArr[3] = 2;//data- kekId
	writeArr[4] = 0xffff;//crc
	return writeArr;
}

uint8_t* MockMSPFunctions::Encrypt_data_key(long userId, uint8_t kekId, uint8_t key_length, uint8_t* arrRandom)
{
	uint8_t* writeArr = new uint8_t[4+key_length];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = key_length;//data Length
	memcpy(writeArr + 3, arrRandom, key_length);//data- encryptedKey
	writeArr[key_length + 3] = 0xffff;//crc
	return writeArr;
}

uint8_t* MockMSPFunctions::Decrypt_data_key(long userId, uint8_t kekId, uint8_t encryptedKeyLength, uint8_t* encryptedKey)
{
	uint8_t* writeArr = new uint8_t[4 + encryptedKeyLength];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = encryptedKeyLength;//data Length
	memcpy(writeArr + 3, encryptedKey, encryptedKeyLength);//data- clear key
	writeArr[encryptedKeyLength+3] = 0xffff;//crc
	return writeArr;
}

//uint8_t* MockMSPFunctions::read(int magic, int command, int dataLength, int* data, int crc)
//{
//	uint8_t* writeArr = new uint8_t[5];
//	writeArr[0] = MAGIC;
//	//writeArr[1] = IMSPFunctions::SUCCESS;
//	switch (command)
//	{
//	case Get_Kek_By_Info:
//		writeArr[2] = 1;
//
//	default:
//		break;
//	}
//}

//void MockMSPFunctions::write(int* magic, int* status, int* dataLength, int* result, int* crc)
//{
//	*status = IMSPFunctions::SUCCESS;
//
//}

