#include "Generate_Key.h"

int gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	static int keyId = 0; //give the key id
	keyId++;

	CK_RV rv;
	int key_length = 0;
	do
	{
		std::cout << "enter the length of the key: 32/ 16/ 24 \n";
		std::cin >> key_length;
	} while (key_length != 32 && key_length != 16 && key_length != 24);

	//////////////////////////////////////////////////////////////////////
	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();
	//int status = GetRandom(arrRandom, key_length);
	//if (status != kStatus_SSS_Success)
	//{
	//	std::cerr << "GetRandom failed";
	//	exit(1);
	//}

	//call MSP
	uint8_t kekId = 2; /*Get - Kek - By - Info(MAGIC, GET_KEK_BT_INFO, data_lenght, (userId, kekInfo from the token), crc);
	uint8_t* arrEncrypted = Encrypt - data - key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, key_length, arrRandom), crc);*/
	//////////////////////////////////////////////////////////////////////

	unsigned char* countBytes = reinterpret_cast<unsigned char*>(&keyId);
	unsigned char* kekIdBytes = reinterpret_cast<unsigned char*>(&kekId);
	CK_OBJECT_CLASS secret_key_class = CKO_SECRET_KEY;
	CK_BBOOL _true = TRUE;
	CK_KEY_TYPE keyType = CKK_AES;

	CK_ATTRIBUTE templateKey[20] = {
		{CKA_CLASS, &secret_key_class, sizeof(secret_key_class)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &_true, sizeof(_true)},
		{CKA_ID, arrRandom /*arrEncrypted*/, key_length},
		{CKA_LABEL, kekIdBytes, sizeof(kekIdBytes)},
		//{CKA_VALUE_LEN, &key_length, sizeof(key_length) }
	};

	// Create the object
	rv = hsm->C_CreateObject(session, templateKey, 5, hSecretKey);

	if (rv != CKR_OK) {
		std::cerr << "Failed to create the object" << std::endl;
		return NULL;
	}

	printf("Successfully created the key \n");

	delete[] arrRandom;

	return keyId;
}

