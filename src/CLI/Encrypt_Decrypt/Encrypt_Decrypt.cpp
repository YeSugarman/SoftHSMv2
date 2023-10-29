
#include "Encrypt_Decrypt.h"

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

	CK_ULONG* arrRandom = new CK_ULONG[MAX_LENGTH];
	CK_ULONG kekIdBytes = 0;

	CK_ATTRIBUTE template_obj[2] = {
	  {CKA_VALUE, arrRandom, MAX_LENGTH},
	  {CKA_LABEL, &kekIdBytes, sizeof(kekIdBytes)}
	};
	CK_ULONG ulcount = 2;

	rv = hsm->C_GetAttributeValue(session, key, template_obj, ulcount);
	if (rv != CKR_OK) {
		std::cerr << "Failed to retrieve CKA_VALUE attribute" << std::endl;
		return;
	}

	//call MSP
	unsigned int rxBuffer[5];
	rxBuffer[0] = MAGIC;
	rxBuffer[1] = 108;/*IMSPFunctions::def_Decrypt_Data_Key;*/
	rxBuffer[2] = 0;
	rxBuffer[3] = 3;// data length
	rxBuffer[4] = 'a';// userId, kekId, encryptedKeyLength, encryptedKey
	unsigned int crc = callCrc(rxBuffer, 5);
	//uint8_t* result = MSPProvider->Decrypt_data_key(, crc);
	

	uint8_t* clearKey = new uint8_t[32]();

	CK_OBJECT_CLASS class_obj = CKO_SECRET_KEY;
	CK_BBOOL true_obj = CK_TRUE;
	CK_KEY_TYPE  type = CKK_AES;

	CK_ATTRIBUTE template_obj2[] = {
	  {CKA_CLASS, &class_obj, sizeof(class_obj)},
	  {CKA_KEY_TYPE, &type, sizeof(type)},
	  {CKA_TOKEN, &true_obj, sizeof(true_obj)},
	  {CKA_VALUE, clearKey, MAX_LENGTH}
	};
	ulcount = 4;
	CK_OBJECT_HANDLE hSecretKey;
	rv = hsm->C_CreateObject(session, template_obj2, ulcount, &hSecretKey);
	if (rv != CKR_OK)
	{
		std::cout << "ERROR in create object" << std::endl;
	}

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
		rv = hsm->C_EncryptInit(session, &mech, hSecretKey);
		if (rv != CKR_OK) {
			std::cerr << "C_EncryptInit failed" << rv << std::endl;
			return;
		}

		out_len = sizeof(out_buffer);
		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}

	if (rv != CKR_OK) {
		rv = hsm->C_EncryptInit(session, &mech, hSecretKey);
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

	rv = hsm->C_DestroyObject( session, hSecretKey);
	if (rv != CKR_OK) {
		std::cerr << "C_DestroyObject failed" << rv << std::endl;
		return;
	}

	std::cout << "The encryption process was successful" << std::endl;

}

void decrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file) {

	CK_RV rv;

	CK_ULONG* arrRandom = new CK_ULONG[MAX_LENGTH];
	CK_ULONG kekIdBytes = 0;

	CK_ATTRIBUTE template_obj[2] = {
	  {CKA_VALUE, arrRandom, MAX_LENGTH},
	  {CKA_LABEL, &kekIdBytes, sizeof(kekIdBytes)}
	};
	CK_ULONG ulcount = 2;

	rv = hsm->C_GetAttributeValue(session, key, template_obj, ulcount);
	if (rv != CKR_OK) {
		std::cerr << "Failed to retrieve CKA_VALUE attribute" << std::endl;
		return;
	}

	//call MSP
	unsigned int rxBuffer[5];
	rxBuffer[0] = MAGIC;
	rxBuffer[1] = 108;/*IMSPFunctions::def_Decrypt_Data_Key;*/
	rxBuffer[2] = 0;
	rxBuffer[3] = 3;// data length
	rxBuffer[4] = 'a';// userId, kekId, encryptedKeyLength, encryptedKey
	unsigned int crc = callCrc(rxBuffer, 5);
	//uint8_t* result = MSPProvider->Decrypt_data_key(, crc);
	

	uint8_t* clearKey = new uint8_t[32]();

	CK_OBJECT_CLASS class_obj = CKO_SECRET_KEY;
	CK_BBOOL true_obj = CK_TRUE;
	CK_KEY_TYPE  type = CKK_AES;

	CK_ATTRIBUTE template_obj2[] = {
	  {CKA_CLASS, &class_obj, sizeof(class_obj)},
	  {CKA_KEY_TYPE, &type, sizeof(type)},
	  {CKA_TOKEN, &true_obj, sizeof(true_obj)},
	  {CKA_VALUE, clearKey, MAX_LENGTH}
	};
	ulcount = 4;
	CK_OBJECT_HANDLE hSecretKey;
	rv = hsm->C_CreateObject(session, template_obj2, ulcount, &hSecretKey);
	if (rv != CKR_OK)
	{
		std::cout << "ERROR in create object" << std::endl;
	}

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
	rv = hsm->C_DecryptInit(session, &mech, hSecretKey);
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

	rv = hsm->C_DestroyObject( session, hSecretKey);
	if (rv != CKR_OK) {
		std::cerr << "C_DestroyObject failed" << rv << std::endl;
		return;
	}

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

CK_OBJECT_HANDLE findKey(CK_SESSION_HANDLE session, CK_ULONG keyId)
{
	CK_RV rv;

	CK_OBJECT_HANDLE keys[1]; 

	CK_ATTRIBUTE templateKey[] = {
		{CKA_OBJECT_ID, &keyId, sizeof(keyId)}
	};
	rv = hsm->C_FindObjectsInit(session, templateKey, sizeof(templateKey) / sizeof(CK_ATTRIBUTE));
	if (rv != CKR_OK) {
		std::cerr << "Failed to initialize object search" << rv << std::endl;
		return NULL;
	}

	// Find the objects
	CK_ULONG ulObjectCount;
	rv = hsm->C_FindObjects(session, keys, sizeof(keys) / sizeof(CK_OBJECT_HANDLE), &ulObjectCount);
	if (rv != CKR_OK) {
		std::cerr << "Failed to find objects" << rv << std::endl;
		return NULL;
	}

	// Finalize the object search
	rv = hsm->C_FindObjectsFinal(session);
	if (rv != CKR_OK) {
		std::cerr << "Failed to finalize object search" << rv << std::endl;
		return NULL;
	}

	return keys[0];
}

