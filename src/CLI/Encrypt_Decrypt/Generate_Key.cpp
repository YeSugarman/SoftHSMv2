#include "Generate_Key.h"

CK_ULONG gen_key(int key_length, CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	static CK_ULONG count = 0;
	count++;

	CK_RV rv;
	INXPFunctions::sss_status_t status = INXPFunctions::sss_status_t::kStatus_SSS_Success;

	//////////////////////////////////////////////////////////////////////
	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();

	status = NXPProvider->GetRandom(arrRandom, key_length);

	//call MSP
	uint8_t kekId = 2; /*Get - Kek - By - Info(MAGIC, GET_KEK_BT_INFO, data_lenght, (userId, kekInfo from the token), crc);
	uint8_t* arrEncrypted = Encrypt - data - key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, key_length, arrRandom), crc);*/
	//////////////////////////////////////////////////////////////////////

	for (int i = 0; i < key_length; i++) {
		arrRandom[i] = static_cast<uint8_t>(10);
	}

	CK_OBJECT_CLASS class_obj = CKO_DATA;
	CK_BBOOL true_obj = CK_TRUE;
	CK_ATTRIBUTE template_obj[] = {
	  {CKA_CLASS, &class_obj, sizeof(class_obj)},
	  {CKA_TOKEN, &true_obj, sizeof(true_obj)},
	  {CKA_VALUE, arrRandom, key_length},
	  {CKA_LABEL, &kekId, sizeof(kekId)},
	  {CKA_OBJECT_ID,&count,sizeof(count)}
	};
	CK_ULONG ulcount = 5;
	rv = hsm->C_CreateObject(session, template_obj, ulcount, hSecretKey);
	if (rv != CKR_OK)
	{
		std::cout << "ERROR in create object" << std::endl;
	}

	printf("Successfully created the key \n");

	delete[] arrRandom;

	return count;
}

