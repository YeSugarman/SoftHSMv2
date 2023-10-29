#include "Generate_Key.h"

CK_ULONG gen_key(int key_length, CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	static CK_ULONG count = 0;
	count++;

	CK_RV rv;
	INXPFunctions::sss_status_t status = INXPFunctions::sss_status_t::kStatus_SSS_Success;

	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();
	status = NXPProvider->GetRandom(arrRandom, key_length);

	//call MSP
	unsigned int rxBuffer[5];
	rxBuffer[0] = MAGIC;
	rxBuffer[1] = 105;/*IMSPFunctions::def_Get_Kek_By_Info;*/
	rxBuffer[2] = 0;
	rxBuffer[3] = 3;// data length
	rxBuffer[4] = 'a';// data-userId, kekInfo
	unsigned int crc = callCrc(rxBuffer,5);
	//uint8_t* result = MSPProvider->Get_Kek_By_Info(, crc);
	uint8_t kekId = 2; 

	rxBuffer[0] = MAGIC;
	rxBuffer[1] = 107;/*IMSPFunctions::def_Encrypt_Data_Key;*/
	rxBuffer[2] = 0;
	rxBuffer[3] = 3;// data length
	rxBuffer[4] = 'a';// userId, kekId, key_length, arrRandom
	crc = callCrc(rxBuffer,5);
	//result = MSPProvider->Encrypt_data_key(, crc);

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

