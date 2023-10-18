#pragma once
#include "Generate_Key.h"

int gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	static int count = 0; //give the key id
	count++;

	CK_RV rv;
	int key_length = 0;
	do
	{
		std::cout << "enter the length of the key: 32/ 16/ 24 \n";
		std::cin >> key_length;
	} while (key_length != 32 && key_length != 16 && key_length != 24);

	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();
	//int status = GetRandom(arrRandom, key_length);
	//if (status != kStatus_SSS_Success)
	//{
	//	std::cerr << "GetRandom failed";
	//	exit(1);
	//}

	//call MSP
	/*uint8_t kekId = Get - Kek - By - Info(MAGIC, GET_KEK_BT_INFO, data_lenght, (userId, kekInfo from the token), crc);
	uint8_t* arrEncrypted = Encrypt - data - key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, key_length, arrRandom), crc);*/
	uint8_t kekId = 2;

	//cast to byte
	unsigned char countBytes[sizeof(int)];
	unsigned char kekIdBytes[sizeof(int)];
	intToBytes(count, countBytes);
	intToBytes(kekId, kekIdBytes);

	CK_OBJECT_CLASS secret_key_class = CKO_SECRET_KEY;
	CK_KEY_TYPE keyType = CKK_AES; // Correctly typed value for CKA_KEY_TYPE
	CK_BBOOL trueValue = CK_FALSE;
	CK_ATTRIBUTE templateKey[] = {
		{CKA_CLASS, &secret_key_class, sizeof(secret_key_class)},
		{CKA_ID, countBytes, sizeof(countBytes)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},  // Correctly typed value
		{CKA_VALUE, arrRandom /*arrEncrypted*/, key_length},
		{CKA_LABEL, kekIdBytes, sizeof(kekIdBytes)},
		{CKA_PRIVATE, &trueValue, sizeof(trueValue)} // Mark the key as private
	};

	// Create the object
	rv = hsm->C_CreateObject(session, templateKey, sizeof(templateKey) / sizeof(CK_ATTRIBUTE), hSecretKey);

	if (rv != CKR_OK) {
		std::cerr << "Failed to create the object" << std::endl;
		exit(1);
	}

	printf("Successfully created the key \n");

	delete[] arrRandom;

	return count;
}

