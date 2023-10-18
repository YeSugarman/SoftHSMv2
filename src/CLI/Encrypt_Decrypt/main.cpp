
#pragma once

#include "Generate_Key.h"
#include "Encrypt_Decrypt.h"
#include "Initialization_Finalization.h"

int main()
{
	long slotNumber = 0x48b28fb9;
	char* password = "123456789";
	std::cout << "Enter the number of your slot and your password\n";
	std::cout << " slot";
	//std::cin >> slotNumber;
	std::cout << "\n password \n";
	//std::cin >> password;

	CK_SESSION_HANDLE session = InitSession(slotNumber, password);
	CK_OBJECT_HANDLE secretKey = NULL;
	int choose = 0;
	std::cout << "Generate key press 1 \nEncrypt press 2 \nDecrypt press 3 \nExit press 0 \n";
	std::cin >> choose;

	while (choose)
	{
		switch (choose)
		{
		case 1:
			gen_key(slotNumber, session, &secretKey);
			std::cout << "Id key: " << secretKey << "\n";
			break;
		case 2:
		case 3:
			encryptionDecryptionShell(choose, session, findKey(session));
			break;
		default:
			std::cout << "The choose is not correct \n";
			break;
		}
		std::cout << "Generate key press 1 \n Encrypt press 2 \n Decrypt press 3 \n Exit press 0 \n";
		std::cin >> choose;

	}

	CloseSession(session);

	return 0;
}