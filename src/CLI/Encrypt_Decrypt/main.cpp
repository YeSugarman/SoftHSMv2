#include "../../../src/lib/SoftHSM.h"
//#include <corecrt_io.h>// Include the correct SoftHSM header
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>

typedef enum
{
	/** Operation was successful */
	kStatus_SSS_Success = 0x5a5a5a5au,
	/** Operation failed */
	kStatus_SSS_Fail = 0x3c3c0000u,
	/** Operation not performed because some of the passed parameters
	 * were found inappropriate */
	kStatus_SSS_InvalidArgument = 0x3c3c0001u,
	// LCOV_EXCL_START
	/** Where the underlying sub-system supports multi-threading,
	 * Internal status to handle simultaneous access.
	 *
	 * This status is not expected to be returned to higher layers.
	 * */
	kStatus_SSS_ResourceBusy = 0x3c3c0002u,
	// LCOV_EXCL_STOP
} sss_status_t;

namespace fs = std::filesystem;

#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200

#define MAGIC "35"
#define GET_KEK_BT_INFO "105"
#define ENCRYPT_DATA_KEY "107"
#define DECRYPT_DATA_KEY "108"

// Create an instance of SoftHSM
SoftHSM* hsm = SoftHSM::i(); // Get the instance

void intToBytes(int value, unsigned char* bytes) {
	for (int i = 0; i < sizeof(int); i++) {
		bytes[i] = (value >> (8 * i)) & 0xFF;
	}
}

int gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey)
{
	static int count = 0; //give the key id
	count++;

	CK_RV rv;
	int key_length = 0;
	do
	{
		std::cout << "enter the length of the key: 32/ 16/ 24 \n";
		std::cin >> key_length;
	} while (key_length != 32 && key_length != 16 && key_length != 24);
	
	//call NXP
	uint8_t* arrRandom = new uint8_t[key_length]();
	int status = GetRandom(arrRandom, key_length);
	if (status != kStatus_SSS_Success)
	{
		std::cerr << "GetRandom failed";
		exit(1);
	}

	//call MSP
	uint8_t kekId = Get - Kek - By - Info(MAGIC, GET_KEK_BT_INFO, data_lenght, (userId, kekInfo from the token), crc);
	uint8_t * arrEncrypted = Encrypt - data - key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, key_length, arrRandom), crc);

	//cast to byte
	unsigned char* countBytes;
	unsigned char* kekIdBytes;
	intToBytes(count, countBytes);
	intToBytes(kekId, kekIdBytes);

	// Define a template for the object
	CK_OBJECT_CLASS secret_key_class = CKO_SECRET_KEY;
	CK_ATTRIBUTE templateKey[] = {
		{CKA_CLASS, &secret_key_class, sizeof(secret_key_class)},
		{CKA_ID, countBytes, sizeof(countBytes)},
		{CKA_VALUE, arrEncrypted, key_length},
		{CKA_LABEL, kekIdBytes, sizeof(kekIdBytes)}
	};

	// Create the object
	rv = hsm->C_CreateObject(session, templateKey, sizeof(templateKey) / sizeof(CK_ATTRIBUTE), hSecretKey);

	if (rv != CKR_OK) {
		std::cerr << "Failed to create the object" << std::endl;
		exit(1);
	}

	printf("Successfully created the key \n");

	delete[] countBytes;
	delete[] kekIdBytes;
	delete[] arrRandom;

	return count;
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

void encrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file)
{
	CK_RV rv;

	// Define an attribute template to request the CKA_VALUE attribute
	CK_ATTRIBUTE templateValue[] = {
		{CKA_VALUE, NULL, 0}, // Placeholder for the value
		{CKA_LABEL, NULL, 0}
	};

	// Retrieve the CKA_VALUE attribute
	rv = C_GetAttributeValue(session, key, templateValue, sizeof(templateValue) / sizeof(CK_ATTRIBUTE));
	if (rv != CKR_OK) {
		std::cerr << "Failed to retrieve CKA_VALUE attribute" << std::endl;
		exit(1);
	}
	// The CKA_VALUE attribute is now stored in template[0]
	// You can access the value using template[0].pValue and template[0].ulValueLen
	uint8_t* encryptedKey = static_cast<CK_BYTE*>(templateValue[0].pValue);
	uint8_t* kekId = static_cast<CK_BYTE*>(templateValue[1].pValue);
	CK_ULONG encryptedKeyLength = templateValue[0].ulValueLen;

	//call MSP
	uint8_t* clearKey = Decrypt-data-key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, encryptedKeyLength, encryptedKey), crc);

	CK_OBJECT_HANDLE object_handle = reinterpret_cast<CK_OBJECT_HANDLE>(clearKey);

	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
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
		rv = hsm->C_EncryptInit(session, &mech, clearKeyLocal);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptInit failed" << rv << std::endl;
			return;
		}

		out_len = sizeof(out_buffer);
		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}

	if (rv != CKR_OK) {
		rv = hsm->C_EncryptInit(session, &mech, clearKeyLocal);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptInit failed" << rv << std::endl;
			return;
		}

		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_EncryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK) {
				std::cerr << "C_EncryptUpdate failed" << rv << std::endl;
				return;
			}
			out_stream.write(reinterpret_cast<char*>(out_buffer), out_len);
			r = in_stream.readsome(reinterpret_cast<char*>(in_buffer), sizeof(in_buffer));
			in_len = r;
		} while (r > 0);

		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptFinal failed" << rv << std::endl;
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
	
	CK_RV rv;

	// Define an attribute template to request the CKA_VALUE attribute
	CK_ATTRIBUTE templateValue[] = {
		{CKA_VALUE, NULL, 0}, // Placeholder for the value
		{CKA_LABEL, NULL, 0}
	};

	// Retrieve the CKA_VALUE attribute
	rv = C_GetAttributeValue(session, key, templateValue, sizeof(templateValue) / sizeof(CK_ATTRIBUTE));
	if (rv != CKR_OK) {
		std::cerr << "Failed to retrieve CKA_VALUE attribute" << std::endl;
		exit(1);
	}
	// The CKA_VALUE attribute is now stored in template[0]
	// You can access the value using template[0].pValue and template[0].ulValueLen
	uint8_t* encryptedKey = static_cast<CK_BYTE*>(templateValue[0].pValue);
	uint8_t* kekId = static_cast<CK_BYTE*>(templateValue[1].pValue);
	CK_ULONG encryptedKeyLength = templateValue[0].ulValueLen;

	//call MSP
	uint8_t* clearKey = Decrypt - data - key(MAGIC, ENCRYPT_DATA_KEY, data_lenght, (userId, kekId, encryptedKeyLength, encryptedKey), crc);

	CK_OBJECT_HANDLE object_handle = reinterpret_cast<CK_OBJECT_HANDLE>(clearKey);
	
	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(&iv_size);

	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;
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
	rv = hsm->C_DecryptInit(session, &mech, clearKeyLocal);
	if (rv != CKR_OK) {
		std::cerr << "C_DecryptInit failed" << rv << std::endl;
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
			std::cerr << "C_DecryptUpdate failed" << rv << std::endl;
			return;
		}

		file_output.write(reinterpret_cast<char*>(out_buffer), out_len);
	} while (file_input);

	// Finalize decryption
	out_len = sizeof(out_buffer);
	rv = hsm->C_DecryptFinal(session, out_buffer, &out_len);
	if (rv != CKR_OK) {
		std::cerr << "C_DecryptFinal failed" << rv << std::endl;
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
	std::string opt_input;
	std::string opt_output;

	std::cout << "Enter path input \n";
	//std::cin >> opt_input;
	std::cout << "Enter path output \n";
	//std::cin >> opt_input;

	if (choose == 2)
		encrypt_data(session, key, opt_input, opt_output);
	else
		decrypt_data(session, key, opt_input, opt_output);
}
CK_OBJECT_HANDLE findKey(CK_SESSION_HANDLE session)
{
	CK_RV rv;
	char* keyId;
	CK_OBJECT_HANDLE keys[1]; // Adjust the array size as needed

	do
	{
		std::cout << "enter your id key \n";
		std::cin >> keyId;//????????????

		CK_ATTRIBUTE templateKey[] = {
			{CKA_LABEL, keyId, sizeof(keyId)}
		};
		rv = hsm->C_FindObjectsInit(session, templateKey, sizeof(templateKey) / sizeof(CK_ATTRIBUTE));
		if (rv != CKR_OK) {
			std::cerr << "Failed to initialize object search" << rv << std::endl;
			exit(1);
		}

		// Find the objects
		CK_ULONG ulObjectCount;
		rv = C_FindObjects(session, keys, sizeof(keys) / sizeof(CK_OBJECT_HANDLE), &ulObjectCount);
		if (rv != CKR_OK) {
			std::cerr << "Failed to find objects" << rv << std::endl;
			exit(1);
		}

		// Finalize the object search
		rv = C_FindObjectsFinal(session);
		if (rv != CKR_OK) {
			std::cerr << "Failed to finalize object search" << rv << std::endl;
			exit(1);
		}
	} while (keys[0]==NULL);

	return keys[0];
}

int main()
{
	long slotNumber = 0x48b28fb9;
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