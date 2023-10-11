
#include <iostream>
#include "../../../src/lib/SoftHSM.h"
#include <corecrt_io.h>// Include the correct SoftHSM header
#include <fcntl.h>
#include <cstdlib>
#include <vector>
#include <cstdio>
//#include "Encrypt_Decrypt/Generate_Key.cpp"
//#include "Encrypt_Decrypt/Initialization_Finalization.cpp"

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

CK_SESSION_HANDLE Initialization(long slotNumber, char* password)
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

void Finalization(CK_SESSION_HANDLE session)
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

void encrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, const char* opt_input, const char* opt_output)
{
	opt_input = "C:\\input.txt";
	opt_output = "C:\\output.txt";
	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(&iv_size);
	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;

	CK_RV rv;
	CK_ULONG in_len, out_len;
	int fd_in, fd_out;
	int r;

	if (opt_input == NULL)
		fd_in = 0;
	else if ((fd_in = open(opt_input, O_RDONLY | O_BINARY)) < 0)
		printf("Cannot open %s: %m", opt_input);

	if (opt_output == NULL) {
		fd_out = 1;
	}
	else {
		fd_out = open(opt_output, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IRUSR | S_IWUSR);
		if (fd_out < 0)
			printf("failed to open %s: %m", opt_output);
	}
	r = read(fd_in, in_buffer, sizeof(in_buffer));
	in_len = r;

	if (r < 0)
		printf("Cannot read from %s: %m", opt_input);

	rv = CKR_CANCEL;
	if (r < (int)sizeof(in_buffer)) {
		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK)
		{
			printf("C_EncryptInit failed \n", rv);
			return;
		}
		out_len = sizeof(out_buffer);
		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}
	if (rv != CKR_OK) {
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK)
		{
			printf("C_EncryptInit failed \n", rv);
			return;
		}
		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_EncryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK)
			{
				printf("C_EncryptUpdate failed \n", rv);
				return;
			}
			r = write(fd_out, out_buffer, out_len);
			if (r != (int)out_len)
				printf("Cannot write to %s: %m", opt_output);
			r = read(fd_in, in_buffer, sizeof(in_buffer));
			in_len = r;
		} while (r > 0);
		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK)
		{
			printf("C_EncryptFinal failed \n", rv);
			return;
		}
	}
	if (out_len) {
		r = write(fd_out, out_buffer, out_len);
		if (r != (int)out_len)
			printf("Cannot write to %s: %m", opt_output);
	}
	if (fd_in != 0)
		close(fd_in);
	if (fd_out != 1)
		close(fd_out);

	free(iv);

	std::cout << "The encryption process was successful \n";
}

static void decrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, char* opt_input, char* opt_output)
{
	opt_input = "C:\\output.txt";
	opt_output = "C:\\result.txt";
	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(&iv_size);
	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;

	CK_RV rv;
	CK_RSA_PKCS_OAEP_PARAMS oaep_params;
	CK_ULONG in_len, out_len;
	int fd_in, fd_out;
	int r;


	if (opt_input == NULL)
		fd_in = 0;
	else if ((fd_in = open(opt_input, O_RDONLY | O_BINARY)) < 0)
		printf("Cannot open %s: %m", opt_input);

	if (opt_output == NULL) {
		fd_out = 1;
	}
	else {
		fd_out = open(opt_output, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IRUSR | S_IWUSR);
		if (fd_out < 0)
			printf("failed to open %s: %m", opt_output);
	}

	r = read(fd_in, in_buffer, sizeof(in_buffer));
	in_len = r;

	if (r < 0)
		printf("Cannot read from %s: %m", opt_input);

	rv = CKR_CANCEL;
	if (r < (int)sizeof(in_buffer)) {
		out_len = sizeof(out_buffer);
		rv = hsm->C_DecryptInit(session, &mech, key);
		if (rv != CKR_OK)
		{
			printf("C_DecryptInit failed \n", rv);
			return;
		}
		rv = hsm->C_Decrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}
	if (rv != CKR_OK) {
		rv = hsm->C_DecryptInit(session, &mech, key);
		if (rv != CKR_OK)
		{
			printf("C_DecryptInit failed \n", rv);
			return;
		}
		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_DecryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK)
			{
				printf("C_DecryptUpdate failed \n", rv);
				return;
			}
			r = write(fd_out, out_buffer, out_len);
			if (r != (int)out_len)
				printf("Cannot write to %s: %m", opt_output);
			r = read(fd_in, in_buffer, sizeof(in_buffer));
			in_len = r;
		} while (r > 0);
		out_len = sizeof(out_buffer);
		rv = hsm->C_DecryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK)
		{
			printf("C_DecryptFinal failed \n", rv);
			return;
		}
	}
	if (out_len) {
		r = write(fd_out, out_buffer, out_len);
		if (r != (int)out_len)
			printf("Cannot write to %s: %m", opt_output);
	}
	if (fd_in != 0)
		close(fd_in);
	if (fd_out != 1)
		close(fd_out);

	free(iv);

	std::cout << "The decryption process was successful \n";

}

void encryptionDecryptionShell(int choose, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key)
{
	char* opt_input = NULL;
	char* opt_output = NULL;

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

	CK_SESSION_HANDLE session = Initialization(slotNumber,password);
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
	
	Finalization(session);

	return 0;
}