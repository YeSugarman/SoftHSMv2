
#include <iostream>
#include "../../../src/lib/SoftHSM.h"
#include <corecrt_io.h>// Include the correct SoftHSM header
#include <fcntl.h>
#include <cstdlib>
//#include "SoftHSM.h"

#include "Encrypt_Decrypt/Encrypt_Decrypt.cpp"
#include "Encrypt_Decrypt/Generate_Key.cpp"
#include "Encrypt_Decrypt/Initialization_Finalization.cpp"

#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200

int main()
{
	// Create an instance of SoftHSM
	SoftHSM* hsm = SoftHSM::i(); // Get the instance
	char* opt_input = NULL;
	char* opt_output = NULL;
	CK_OBJECT_HANDLE secretKey;

	long slotNumber = 1219661753;
	char* password = "123456789";
	std::cout << "Enter the number of your slot and your password\n";
	/*std::cout << "slot";
	std::cin >> slotNumber;
	std::cout << "\n password";
	std::cin >> password;*/

	CK_SESSION_HANDLE session = Initialization(hsm, slotNumber,password);

	int choose = 0;
	std::cout << "Generate key press 1 \n Encrypt press 2 \n Decrypt press 3 \n Exit press 0 \n";
	std::cin >> choose;

	while (choose)
	{
		switch (choose)
		{
		case 1:
			//gen_key(hsm, slotNumber, session, &secretKey);
			break;
		case 2:
		case 3:
			//encryptionDecryptionShell(hsm,choose, session, secretKey, opt_input, opt_output);
			break;
		default:
			std::cout << "The choose is not correct \n";
			break;
		}
		std::cout << "Generate key press 1 \n Encrypt press 2 \n Decrypt press 3 \n Exit press 0 \n";
		std::cin >> choose;
	}
	
	Finalization(hsm, session);

	return 0;
}