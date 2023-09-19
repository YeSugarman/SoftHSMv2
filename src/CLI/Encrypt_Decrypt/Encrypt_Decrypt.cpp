//#pragma once
////#include "Encrypt_Decrypt/main.cpp"
//
//#include <iostream>
//#include "../../../src/lib/SoftHSM.h"
//#include <corecrt_io.h>// Include the correct SoftHSM header
//#include <fcntl.h>
//#include <cstdlib>
//
//#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
//#define S_IRUSR 0400
//#define S_IWUSR 0200
//
////// Create an instance of SoftHSM
////SoftHSM* hsm = SoftHSM::i(); // Get the instance
//
//CK_BYTE_PTR get_iv(size_t* iv_size)
//{
//	CK_BYTE_PTR iv;
//
//	iv = (CK_BYTE_PTR)calloc(sizeof(CK_BYTE), *iv_size);
//	if (!iv) {
//		fprintf(stderr, "Warning, out of memory, IV will not be used.\n");
//		*iv_size = 0;
//		return NULL;
//	}
//
//	return iv;
//}
//
//void encrypt_data(SoftHSM* hsm, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, const char* opt_input, const char* opt_output)
//{
//	unsigned char in_buffer[1024], out_buffer[1024];
//
//	CK_MECHANISM mech;
//	size_t iv_size = 16;
//	CK_BYTE_PTR iv = get_iv(&iv_size);
//	mech.mechanism = CKM_AES_CBC_PAD;
//	mech.pParameter = iv;
//	mech.ulParameterLen = iv_size;
//
//	CK_RV rv;
//	CK_ULONG in_len, out_len;
//	int fd_in, fd_out;
//	int r;
//
//	if (opt_input == NULL)
//		fd_in = 0;
//	else if ((fd_in = open(opt_input, O_RDONLY | O_BINARY)) < 0)
//		printf("Cannot open %s: %m", opt_input);
//
//	if (opt_output == NULL) {
//		fd_out = 1;
//	}
//	else {
//		fd_out = open(opt_output, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IRUSR | S_IWUSR);
//		if (fd_out < 0)
//			printf("failed to open %s: %m", opt_output);
//	}
//	r = read(fd_in, in_buffer, sizeof(in_buffer));
//	in_len = r;
//
//	if (r < 0)
//		printf("Cannot read from %s: %m", opt_input);
//
//	rv = CKR_CANCEL;
//	if (r < (int)sizeof(in_buffer)) {
//		out_len = sizeof(out_buffer);
//		rv = hsm->C_EncryptInit(session, &mech, key);
//		if (rv != CKR_OK)
//			printf("C_EncryptInit failed \n", rv);
//		out_len = sizeof(out_buffer);
//		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
//	}
//	if (rv != CKR_OK) {
//		rv = hsm->C_EncryptInit(session, &mech, key);
//		if (rv != CKR_OK)
//			printf("C_EncryptInit failed \n", rv);
//		do {
//			out_len = sizeof(out_buffer);
//			rv = hsm->C_EncryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
//			if (rv != CKR_OK)
//				printf("C_EncryptUpdate failed \n", rv);
//			r = write(fd_out, out_buffer, out_len);
//			if (r != (int)out_len)
//				printf("Cannot write to %s: %m", opt_output);
//			r = read(fd_in, in_buffer, sizeof(in_buffer));
//			in_len = r;
//		} while (r > 0);
//		out_len = sizeof(out_buffer);
//		rv = hsm->C_EncryptFinal(session, out_buffer, &out_len);
//		if (rv != CKR_OK)
//			printf("C_EncryptFinal failed \n", rv);
//	}
//	if (out_len) {
//		r = write(fd_out, out_buffer, out_len);
//		if (r != (int)out_len)
//			printf("Cannot write to %s: %m", opt_output);
//	}
//	if (fd_in != 0)
//		close(fd_in);
//	if (fd_out != 1)
//		close(fd_out);
//
//	free(iv);
//
//	std::cout << "The encryption process was successful \n";
//}
//
//static void decrypt_data(SoftHSM* hsm, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, char* opt_input, char* opt_output)
//{
//
//	unsigned char in_buffer[1024], out_buffer[1024];
//
//	CK_MECHANISM mech;
//	size_t iv_size = 16;
//	CK_BYTE_PTR iv = get_iv(&iv_size);
//	mech.mechanism = CKM_AES_CBC_PAD;
//	mech.pParameter = iv;
//	mech.ulParameterLen = iv_size;
//
//	CK_RV rv;
//	CK_RSA_PKCS_OAEP_PARAMS oaep_params;
//	CK_ULONG in_len, out_len;
//	int fd_in, fd_out;
//	int r;
//
//
//	if (opt_input == NULL)
//		fd_in = 0;
//	else if ((fd_in = open(opt_input, O_RDONLY | O_BINARY)) < 0)
//		printf("Cannot open %s: %m", opt_input);
//
//	if (opt_output == NULL) {
//		fd_out = 1;
//	}
//	else {
//		fd_out = open(opt_output, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IRUSR | S_IWUSR);
//		if (fd_out < 0)
//			printf("failed to open %s: %m", opt_output);
//	}
//
//	r = read(fd_in, in_buffer, sizeof(in_buffer));
//	in_len = r;
//
//	if (r < 0)
//		printf("Cannot read from %s: %m", opt_input);
//
//	rv = CKR_CANCEL;
//	if (r < (int)sizeof(in_buffer)) {
//		out_len = sizeof(out_buffer);
//		rv = hsm->C_DecryptInit(session, &mech, key);
//		if (rv != CKR_OK)
//			printf("C_DecryptInit failed \n", rv);
//		rv = hsm->C_Decrypt(session, in_buffer, in_len, out_buffer, &out_len);
//	}
//	if (rv != CKR_OK) {
//		rv = hsm->C_DecryptInit(session, &mech, key);
//		if (rv != CKR_OK)
//			printf("C_DecryptInit failed \n", rv);
//		do {
//			out_len = sizeof(out_buffer);
//			rv = hsm->C_DecryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
//			if (rv != CKR_OK)
//				printf("C_DecryptUpdate failed \n", rv);
//			r = write(fd_out, out_buffer, out_len);
//			if (r != (int)out_len)
//				printf("Cannot write to %s: %m", opt_output);
//			r = read(fd_in, in_buffer, sizeof(in_buffer));
//			in_len = r;
//		} while (r > 0);
//		out_len = sizeof(out_buffer);
//		rv = hsm->C_DecryptFinal(session, out_buffer, &out_len);
//		if (rv != CKR_OK)
//			printf("C_DecryptFinal failed \n", rv);
//	}
//	if (out_len) {
//		r = write(fd_out, out_buffer, out_len);
//		if (r != (int)out_len)
//			printf("Cannot write to %s: %m", opt_output);
//	}
//	if (fd_in != 0)
//		close(fd_in);
//	if (fd_out != 1)
//		close(fd_out);
//
//	free(iv);
//
//	std::cout << "The decryption process was successful \n";
//
//}
//
//void encryptionDecryptionShell(SoftHSM* hsm, int choose, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, char* opt_input, char* opt_output)
//{
//	if (key == NULL)
//	{
//		std::cout << "There is no key, pleas generate key before encryption";
//		exit(1);
//	}
//
//	opt_input = "C:\\input.txt";
//	opt_output = "C:\\output.txt";
//
//	//std::cout << "Enter path input \n";
//	//std::cin >> opt_input;
//	//std::cout << "Enter path output \n";
//	//std::cin >> opt_input;
//
//	if (choose == 2)
//		encrypt_data(hsm, session, key, opt_input, opt_output);
//	else
//		decrypt_data(hsm, session, key, opt_input, opt_output);
//}
