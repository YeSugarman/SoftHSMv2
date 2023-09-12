

#include <iostream>
using namespace std;

#include "../../../src/lib/SoftHSM.h"
#include <corecrt_io.h>// Include the correct SoftHSM header
#include <fcntl.h>
#include <cstdlib>
#include <getopt.h>
//#include <getopt.cpp>

//#include "../../../src/bin/keyconv/softhsm2-keyconv.cpp"
#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200

static const char* opt_input = NULL;
static const char* opt_output = NULL;
static const char* opt_iv = optarg;

// Create an instance of SoftHSM
SoftHSM* hsm = SoftHSM::i(); // Get the instance

static CK_BYTE_PTR get_iv(const char* iv_input, size_t* iv_size)
{
	CK_BYTE_PTR iv;
	size_t size = *iv_size;

	/* no IV supplied on command line */
	/*if (!iv_input) {
	*iv_size = 0;
	return NULL;
	}*/

	iv = (CK_BYTE_PTR)calloc(sizeof(CK_BYTE), *iv_size);
	if (!iv) {
		fprintf(stderr, "Warning, out of memory, IV will not be used.\n");
		*iv_size = 0;
		return NULL;
	}

	/*if (sc_hex_to_bin(iv_input, iv, &size)) {
	fprintf(stderr, "Warning, unable to parse IV, IV will not be used.\n");
	*iv_size = 0;
	free(iv);
	return NULL;
	}*/

	if (*iv_size != size)
		fprintf(stderr, "Warning: IV string is too short, IV will be padded from the right by zeros.\n");

	return iv;
}

static void decrypt_data(CK_SESSION_HANDLE session,
	CK_OBJECT_HANDLE key)
{
	opt_input = "C:\\output.txt";
	opt_output = "C:\\result.txt";

	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(opt_iv, &iv_size);
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
			printf("C_DecryptInit", rv);
		/*if (getALWAYS_AUTHENTICATE(session, key))
		login(session, CKU_CONTEXT_SPECIFIC);*/
		rv = hsm->C_Decrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}
	if (rv != CKR_OK) {
		rv = hsm->C_DecryptInit(session, &mech, key);
		if (rv != CKR_OK)
			printf("C_DecryptInit", rv);
		/*if (getALWAYS_AUTHENTICATE(session, key))
		login(session, CKU_CONTEXT_SPECIFIC);*/
		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_DecryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK)
				printf("C_DecryptUpdate", rv);
			r = write(fd_out, out_buffer, out_len);
			if (r != (int)out_len)
				printf("Cannot write to %s: %m", opt_output);
			r = read(fd_in, in_buffer, sizeof(in_buffer));
			in_len = r;
		} while (r > 0);
		out_len = sizeof(out_buffer);
		rv = hsm->C_DecryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK)
			printf("C_DecryptFinal", rv);
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
}


static void encrypt_data(CK_SESSION_HANDLE session,
	CK_OBJECT_HANDLE key)
{
	opt_input = "C:\\input.txt";
	opt_output = "C:\\output.txt";

	unsigned char in_buffer[1024], out_buffer[1024];

	CK_MECHANISM mech;
	size_t iv_size = 16;
	CK_BYTE_PTR iv = get_iv(opt_iv, &iv_size);
	mech.mechanism = CKM_AES_CBC_PAD;
	mech.pParameter = iv;
	mech.ulParameterLen = iv_size;

	CK_RV rv;
	CK_ULONG in_len, out_len;
	int fd_in, fd_out;
	int r;

	//if (!opt_mechanism_used)
	// if (!find_mechanism(slot, CKF_ENCRYPT | opt_allow_sw, NULL, 0, &opt_mechanism))
	// //util_fatal("Encrypt mechanism not supported");
	// printf("Encrypt mechanism not supported");

	//fprintf(stderr, "Using encrypt algorithm %s\n", p11_mechanism_to_name(opt_mechanism));
	//memset(&mech, 0, sizeof(mech));
	//mech.mechanism = opt_mechanism;

	//switch (opt_mechanism) {
	//case CKM_AES_ECB:
	// mech.pParameter = NULL;
	// mech.ulParameterLen = 0;
	// break;
	//case CKM_AES_CBC:
	//case CKM_AES_CBC_PAD:
	// iv_size = 16;
	// iv = get_iv(opt_iv, &iv_size);
	// mech.pParameter = iv;
	// mech.ulParameterLen = iv_size;
	// break;
	//default:
	// //util_fatal("Mechanism %s illegal or not supported\n", p11_mechanism_to_name(opt_mechanism));
	// printf("Mechanism %s illegal or not supported\n", p11_mechanism_to_name(opt_mechanism));
	//}

	if (opt_input == NULL)
		fd_in = 0;
	else if ((fd_in = open(opt_input, O_RDONLY | O_BINARY)) < 0)
		//util_fatal("Cannot open %s: %m", opt_input);
		printf("Cannot open %s: %m", opt_input);

	if (opt_output == NULL) {
		fd_out = 1;
	}
	else {
		fd_out = open(opt_output, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IRUSR | S_IWUSR);
		if (fd_out < 0)
			//util_fatal("failed to open %s: %m", opt_output);
			printf("failed to open %s: %m", opt_output);
	}
	//, S_IRUSR | S_IWUSR
	r = read(fd_in, in_buffer, sizeof(in_buffer));
	in_len = r;

	if (r < 0)
		//util_fatal("Cannot read from %s: %m", opt_input);
		printf("Cannot read from %s: %m", opt_input);

	rv = CKR_CANCEL;
	if (r < (int)sizeof(in_buffer)) {
		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK)
			//p11_fatal("C_EncryptInit", rv);
			printf("C_EncryptInit", rv);
		//if (getALWAYS_AUTHENTICATE(session, key))
		// login(session, CKU_CONTEXT_SPECIFIC);
		out_len = sizeof(out_buffer);
		rv = hsm->C_Encrypt(session, in_buffer, in_len, out_buffer, &out_len);
	}
	if (rv != CKR_OK) {
		rv = hsm->C_EncryptInit(session, &mech, key);
		if (rv != CKR_OK)
			//p11_fatal("C_EncryptInit", rv);
			printf("C_EncryptInit", rv);
		//if (getALWAYS_AUTHENTICATE(session, key))
		// login(session, CKU_CONTEXT_SPECIFIC);
		do {
			out_len = sizeof(out_buffer);
			rv = hsm->C_EncryptUpdate(session, in_buffer, in_len, out_buffer, &out_len);
			if (rv != CKR_OK)
				//p11_fatal("C_EncryptUpdate", rv);
				printf("C_EncryptUpdate", rv);
			r = write(fd_out, out_buffer, out_len);
			if (r != (int)out_len)
				//util_fatal("Cannot write to %s: %m", opt_output);
				printf("Cannot write to %s: %m", opt_output);
			r = read(fd_in, in_buffer, sizeof(in_buffer));
			in_len = r;
		} while (r > 0);
		out_len = sizeof(out_buffer);
		rv = hsm->C_EncryptFinal(session, out_buffer, &out_len);
		if (rv != CKR_OK)
			//p11_fatal("C_EncryptFinal", rv);
			printf("C_EncryptFinal", rv);
	}
	if (out_len) {
		r = write(fd_out, out_buffer, out_len);
		if (r != (int)out_len)
			//util_fatal("Cannot write to %s: %m", opt_output);
			printf("Cannot write to %s: %m", opt_output);
	}
	if (fd_in != 0)
		close(fd_in);
	if (fd_out != 1)
		close(fd_out);

	free(iv);
}

int main() {
	

	// Initialize the library
	CK_RV rv = hsm->C_Initialize(nullptr);
	if (rv != CKR_OK) {
		std::cout << "Error initializing the library" << std::endl;
		exit(1);
	}

	// You need to open a session first before generating the key pair
	CK_SESSION_HANDLE session;
	rv = hsm->C_OpenSession(819892398, CKF_SERIAL_SESSION | CKF_RW_SESSION, nullptr, nullptr, &session);
	if (rv != CKR_OK) {
		std::cout << "Error opening a session" << std::endl;
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	// Check if the user is already logged in
	CK_SESSION_INFO sessionInfo;
	rv = hsm->C_GetSessionInfo(session, &sessionInfo);
	if (rv != CKR_OK) {
		std::cout << "Error getting session info" << std::endl;
		hsm->C_CloseSession(session); // Close the session
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	if (sessionInfo.state == CKS_RO_USER_FUNCTIONS || sessionInfo.state == CKS_RW_USER_FUNCTIONS) {
		std::cout << "User is already logged in" << std::endl;
		hsm->C_CloseSession(session); // Close the session
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	rv = hsm->C_Login(session, CKU_USER, (CK_UTF8CHAR*)"123456789", 9);
	if (rv != CKR_OK) {
		std::cout << "Error logging in to the token" << std::endl;
		hsm->C_CloseSession(session); // Close the session
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	CK_MECHANISM mechanism = { CKM_AES_KEY_GEN, NULL_PTR, 0 };
	CK_OBJECT_CLASS secret_key_class = CKO_SECRET_KEY;
	CK_BBOOL _true = TRUE;
	CK_ATTRIBUTE keyTemplate[20] = {
	{CKA_CLASS, &secret_key_class, sizeof(secret_key_class)},
	{CKA_TOKEN, &_true, sizeof(_true)},
	};
	int n_attr = 2;
	int key_length = 32;
	FILL_ATTR(keyTemplate[n_attr], CKA_VALUE_LEN, &key_length, sizeof(key_length));
	n_attr++;

	CK_OBJECT_HANDLE hSecretKey;

	rv = hsm->C_GenerateKey(session, &mechanism, keyTemplate, n_attr, &hSecretKey);
	if (rv != CKR_OK) {
		std::cout << "Error generating key" << std::endl;
	}
	encrypt_data(session, hSecretKey);
	decrypt_data(session, hSecretKey);

	// Close the session and finalize the library when done
	hsm->C_Logout(session);
	hsm->C_CloseSession(session);
	hsm->C_Finalize(nullptr);

	return 0;
}