#include "MockMSPFunctions.h"

MockMSPFunctions::MockMSPFunctions()
{
}

MockMSPFunctions::~MockMSPFunctions()
{
}

uint8_t* MockMSPFunctions::Get_Kek_By_Info(int dataLenght, int* data, int crc)
{
	uint8_t* writeArr = new uint8_t[5];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = dataLenght;//data length
	writeArr[3] = *data;//data
	writeArr[4] = crc;
	return writeArr;
}

uint8_t* MockMSPFunctions::Encrypt_data_key(int dataLenght, int* data, int crc)
{
	uint8_t* writeArr = new uint8_t[5];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = dataLenght;//data lenght
	writeArr[3] = *data;//data
	writeArr[4] = crc;
	return writeArr;
}

uint8_t* MockMSPFunctions::Decrypt_data_key(int dataLenght, int* data, int crc)
{
	uint8_t* writeArr = new uint8_t[5];
	writeArr[0] = MAGIC;
	writeArr[1] = IMSPFunctions::SUCCESS;
	writeArr[2] = dataLenght;//data lenght
	writeArr[3] = *data;//data
	writeArr[4] = crc;
	return writeArr;
}

//uint8_t* MockMSPFunctions::read(int magic, int command, int dataLenght, int* data, int crc)
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

//void MockMSPFunctions::write(int* magic, int* status, int* dataLenght, int* result, int* crc)
//{
//	*status = IMSPFunctions::SUCCESS;
//
//}

