#pragma once
#include "Generate_Key.h"


int gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	CK_RV rv;
	CK_MECHANISM mechanism = { CKM_AES_KEY_GEN, NULL_PTR, 0 };
	CK_OBJECT_CLASS secret_key_class = CKO_SECRET_KEY;
	CK_BBOOL _true = TRUE;
	CK_ATTRIBUTE keyTemplate[20] = {
	{CKA_CLASS, &secret_key_class, sizeof(secret_key_class)},
	{CKA_TOKEN, &_true, sizeof(_true)},
	};
	int n_attr = 2;
	int key_length = 0;
	do
	{
		std::cout << "enter the length of the key: 32/ 16/ 24 \n";
		std::cin >> key_length;
	} while (key_length != 32 && key_length != 16 && key_length != 24);


	FILL_ATTR(keyTemplate[n_attr], CKA_VALUE_LEN, &key_length, sizeof(key_length));
	n_attr++;

	rv = hsm->C_GenerateKey(session, &mechanism, keyTemplate, n_attr, hSecretKey);
	if (rv != CKR_OK)
	{
		printf("C_GenerateKey failed", rv);
		exit(1);
	}

	printf("Successfully created the key \n");
	//show_key(session, *hSecretKey);
	return 1;
}

