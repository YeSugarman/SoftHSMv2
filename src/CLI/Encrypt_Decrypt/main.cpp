

#include "../../../src/lib/SoftHSM.h"
//#include <corecrt_io.h>// Include the correct SoftHSM header
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>


namespace fs = std::filesystem;


#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200

// Create an instance of SoftHSM
SoftHSM* hsm = SoftHSM::i(); // Get the instance

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

CK_SESSION_HANDLE InitSession(long slotNumber, char* password)
{
	// Initialize the library
	CK_RV rv = hsm->C_Initialize(nullptr);
	if (rv != CKR_OK) {
		std::cout << "Error initializing the library" << std::endl;
		exit(1);
	}

	// You need to open a session first before generating the key pair
	CK_SESSION_HANDLE session;
	rv = hsm->C_OpenSession(slotNumber, CKF_SERIAL_SESSION | CKF_RW_SESSION, nullptr, nullptr, &session);
	if (rv != CKR_OK) {
		std::cout << "Error opening a session" << std::endl;
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	rv = hsm->C_Login(session, CKU_USER, (CK_UTF8CHAR*)password, 9);
	if (rv != CKR_OK) {
		std::cout << "Error logging in to the token" << std::endl;
		hsm->C_CloseSession(session); // Close the session
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}
	return session;
}

void CloseSession(CK_SESSION_HANDLE session)
{
	// Close the session and finalize the library when done
	hsm->C_Logout(session);
	hsm->C_CloseSession(session);
	hsm->C_Finalize(nullptr);
}

CK_BYTE_PTR get_iv(size_t* iv_size)
{
	CK_BYTE_PTR iv;

	iv = (CK_BYTE_PTR)calloc(sizeof(CK_BYTE), *iv_size);
	if (!iv) {
		fprintf(stderr, "Warning, out of memory, IV will not be used.\n");
		*iv_size = 0;
		return NULL;
	}

	return iv;
}

void encrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file) {
	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	CK_RV rv;
	CK_ULONG in_len, out_len;

	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(&iv_size);
	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;
	
	input_file = "C:\\input.txt";
	output_file = "C:\\output.txt";

	std::ifstream in_stream(input_file, std::ios::binary);
	if (!in_stream) {
		std::cerr << "Cannot open input file: " << input_file << std::endl;
		return;
	}

	std::ofstream out_stream(output_file, std::ios::binary);
	if (!out_stream) {
		std::cerr << "Cannot open output file: " << output_file << std::endl;
		return;
	}

	int r;
	in_stream.read(reinterpret_cast<char*>(in_buffer), sizeof(in_buffer));
	in_len = in_stream.gcount();

	rv = CKR_CANCEL;
	if (in_len < sizeof(in_buffer)) {
		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptInit failed" << std::endl;
			return;
		}

		out_len = sizeof(out_buffer);
		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}

	if (rv != CKR_OK) {
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptInit failed" << std::endl;
			return;
		}

		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_EncryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK) {
				std::cerr << "C_EncryptUpdate failed" << std::endl;
				return;
			}
			out_stream.write(reinterpret_cast<char*>(out_buffer), out_len);
			r = in_stream.readsome(reinterpret_cast<char*>(in_buffer), sizeof(in_buffer));
			in_len = r;
		} while (r > 0);

		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptFinal failed" << std::endl;
			return;
		}
	}

	if (out_len) {
		out_stream.write(reinterpret_cast<char*>(out_buffer), out_len);
	}

	in_stream.close();
	out_stream.close();
	std::cout << "The encryption process was successful" << std::endl;
}
void decrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file) {
	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(&iv_size);

	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;
	CK_RV rv;
	CK_ULONG in_len, out_len;

	input_file = "C:\\output.txt";
	output_file = "C:\\result.txt";

	std::ifstream file_input(input_file, std::ios::binary);
	if (!file_input) {
		std::cerr << "Cannot open input file: " << input_file << std::endl;
		return;
	}

	std::ofstream file_output(output_file, std::ios::binary);
	if (!file_output) {
		std::cerr << "Cannot open output file: " << output_file << std::endl;
		return;
	}
	// Initialize decryption
	rv = hsm->C_DecryptInit(session, &mech, key);
	if (rv != CKR_OK) {
		std::cerr << "C_DecryptInit failed" << std::endl;
		return;
	}

	do {
		file_input.read(reinterpret_cast<char*>(in_buffer), sizeof(in_buffer));
		in_len = file_input.gcount();

		if (in_len <= 0) {
			break;  // No more data to decrypt, exit the loop
		}

		rv = hsm->C_DecryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
		if (rv != CKR_OK) {
			std::cerr << "C_DecryptUpdate failed" << std::endl;
			return;
		}

		file_output.write(reinterpret_cast<char*>(out_buffer), out_len);
	} while (file_input);

	// Finalize decryption
	out_len = sizeof(out_buffer);
	rv = hsm->C_DecryptFinal(session, out_buffer, &out_len);
	if (rv != CKR_OK) {
		std::cerr << "C_DecryptFinal failed" << std::endl;
		return;
	}

	if (out_len) {
		file_output.write(reinterpret_cast<char*>(out_buffer), out_len);
	}

	file_input.close();
	file_output.close();

	std::cout << "The decryption process was successful" << std::endl;
}
void encryptionDecryptionShell(int choose, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key)
{
	std::string opt_input = NULL;
	std::string opt_output = NULL;

	std::cout << "Enter path input \n";
	//std::cin >> opt_input;
	std::cout << "Enter path output \n";
	//std::cin >> opt_input;

	if (choose == 2)
		encrypt_data(session, key, opt_input, opt_output);
	else
		decrypt_data(session, key, opt_input, opt_output);
}
CK_OBJECT_HANDLE findKey(std::vector<CK_OBJECT_HANDLE> secretKeys)
{
	int index;
	do
	{
		std::cout << "enter your id key \n";
		std::cin >> index;
	} while (index >= secretKeys.size() + 2);
	return secretKeys[index - 2];
}

int main()
{
	std::vector<CK_OBJECT_HANDLE> secretKeys;

	long slotNumber = 819892398;
	char* password = "123456789";
	std::cout << "Enter the number of your slot and your password\n";
	std::cout << " slot";
	//std::cin >> slotNumber;
	std::cout << "\n password \n";
	//std::cin >> password;

	CK_SESSION_HANDLE session = InitSession(slotNumber,password);
	CK_OBJECT_HANDLE secretKey = NULL;
	int choose = 0;
	std::cout << "Generate key press 1 \nEncrypt press 2 \nDecrypt press 3 \nExit press 0 \n";
	std::cin >> choose;
	while (choose)
	{
		switch (choose)
		{
		case 1:
			gen_key( slotNumber, session,&secretKey);
			std::cout << "Id key: " << secretKey << "\n";
			secretKeys.push_back(secretKey);
			break;
		case 2:
		case 3:
			encryptionDecryptionShell(choose, session, findKey(secretKeys));
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