#include "Generate_Key.h"

void gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
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

	unsigned char* kekIdBytes = reinterpret_cast<unsigned char*>(&kekId);

	for (int i = 0; i < key_length; i++) {
		arrRandom[i] = static_cast<uint8_t>(10);
	}

	CK_OBJECT_CLASS class_obj = CKO_DATA;
	CK_BBOOL true_obj = CK_TRUE;
	CK_ATTRIBUTE template_obj[20] = {
	  {CKA_CLASS, &class_obj, sizeof(class_obj)},
	  {CKA_TOKEN, &true_obj, sizeof(true_obj)},
	  {CKA_VALUE, arrRandom, key_length},
	  {CKA_LABEL, kekIdBytes, sizeof(kekIdBytes)}
	};
	CK_ULONG ulcount = 4;
	rv = hsm->C_CreateObject(session, template_obj, ulcount, hSecretKey);
	if (rv != CKR_OK)
	{
		std::cout << "ERROR in create object" << std::endl;
	}

	printf("Successfully created the key \n");

	delete[] arrRandom;
}

