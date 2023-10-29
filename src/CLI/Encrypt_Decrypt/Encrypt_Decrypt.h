#include "GlobalInstance.h"

extern SoftHSM* hsm;

CK_BYTE_PTR get_iv(size_t* iv_size);
void encrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file);
void decrypt_data(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, std::string& input_file, std::string& output_file);
void encryptionDecryptionShell(int choose, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key);
CK_OBJECT_HANDLE findKey(CK_SESSION_HANDLE session, CK_ULONG keyId);
