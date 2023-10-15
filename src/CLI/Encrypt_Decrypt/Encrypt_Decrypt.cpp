
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
