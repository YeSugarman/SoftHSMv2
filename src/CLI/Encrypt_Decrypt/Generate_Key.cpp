#include "Generate_Key.h"

int gen_key(int key_length, CK_SLOT_ID slot, CK_SESSION_HANDLE session)
{
	CK_RV rv;

	CK_OBJECT_HANDLE secretKey = NULL;

	static int count = 0;
	count++;

	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();
	INXPFunctions::sss_status_t status = NXPProvider->GetRandom(arrRandom, key_length);
	if (status == INXPFunctions::sss_status_t::kStatus_SSS_Fail)
	{
		std::cerr << "GetRandom failed";
		return -1;
	}

	//call MSP
	CK_SESSION_INFO sessionInfo;
	rv = hsm->C_GetSessionInfo(session, &sessionInfo);
	long slotId;
	if (rv != CKR_OK) {
		std::cerr << "C_GetSessionInfo failed";
		return -1;
	}
	else {
		slotId = sessionInfo.slotID;
	}
	std::string kekInfo = "aaa";// we need to get it from the token

	uint8_t* result = MSPProvider->Get_Kek_By_Info(slotId,kekInfo);
	if (result[1] == IMSPFunctions::EXCEPTION)
	{
		std::cerr << "Get_Kek_By_Info failed";
		return -1;
	}
	uint8_t kekId = result[3];

	result = MSPProvider->Encrypt_data_key(slotId,kekId,key_length,arrRandom);
	if (result[1] == IMSPFunctions::EXCEPTION)
	{
		std::cerr << "Encrypt_data_key failed";
		return -1;
	}

	uint8_t* encryptedKey = new uint8_t[key_length];
	memcpy(encryptedKey, result + 3, key_length);

	CK_OBJECT_CLASS class_obj = CKO_DATA;
	CK_BBOOL true_obj = CK_TRUE;
	CK_ATTRIBUTE template_obj[] = {
	  {CKA_CLASS, &class_obj, sizeof(class_obj)},
	  {CKA_TOKEN, &true_obj, sizeof(true_obj)},
	  {CKA_VALUE, encryptedKey, key_length},
	  {CKA_LABEL, &kekId, sizeof(kekId)},
	  {CKA_OBJECT_ID,&count,sizeof(count)}
	};
	CK_ULONG ulcount = 5;
	rv = hsm->C_CreateObject(session, template_obj, ulcount, &secretKey);
	if (rv != CKR_OK)
	{
		std::cout << "ERROR in create object" << std::endl;
		return -1;
	}

	std::cout << "Successfully created the key \n";

	delete[] arrRandom;
	delete[] encryptedKey;
	

	return count;
}

