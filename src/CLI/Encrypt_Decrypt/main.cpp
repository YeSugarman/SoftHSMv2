
#include "Generate_Key.h"
#include "Encrypt_Decrypt.h"
#include "Initialization_Finalization.h"


int main()
{
	/////// try 
	Init();

	int login = -1;
	std::cout << "welcome, to continue press any number, to exit press 0\n";
	std::cin >> login;

	while (login)
	{
		long slotNumber = 169407886;
		char* password = "123456789";
		std::cout << "Enter the number of your slot and your password\n";
		std::cout << " slot";
		//std::cin >> slotNumber;
		std::cout << "\n password \n";
		//std::cin >> password;

		CK_SESSION_HANDLE session = InitSession(slotNumber, password);
		CK_OBJECT_HANDLE secretKey = NULL;
		int choose = 0;
		int keyId = 0;

		while (choose != 4)
		{
			std::cout << "Generate key press 1 \nEncrypt press 2 \nDecrypt press 3 \nExit press 4\n";
			if (!(std::cin >> choose))
			{
				std::cin.clear();  // Clear the error state
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				choose = 0;
			}
			switch (choose)
			{
			case 1:
				gen_key(slotNumber, session, &secretKey);
				std::cout << "Id key: " << secretKey << "\n";
				break;
			case 2:
			case 3:
				encryptionDecryptionShell(choose, session, /*secretKey*/findKey(session));
				break;
			case 4:
				break;
			default:
				std::cout << "The choose is not correct enter again\n";
				break;
			}
		}

		CloseSession(session);

		std::cout << "to continue press any number, to exit 0\n";
		std::cin >> login;
	}

	Final();

	return EXIT_SUCCESS;
}
