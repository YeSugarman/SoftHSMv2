#include "MSPFunctions.h"
#ifdef __linux__

MSPFunctions::MSPFunctions()
{
    handle = dlopen("./mock_MSP.so", RTLD_LAZY);
    if (!handle) {
        cerr << "can not load the library" << endl;
        return;
    }
}

MSPFunctions::~MSPFunctions()
{
    dlclose(handle);
}

uint8_t* Get_Kek_By_Info(long userId, int info_len, std::string kekInfo) 
{
    typedef uint8_t*(*Get_Kek_By_InfoType)(long, int, std::string);

    Get_Kek_By_InfoType Get_Kek_By_Info = (Get_Kek_By_InfoType)dlsym(handle, "Get_Kek_By_Info");
    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Error to read the function: " << dlsymError << std::endl;
        return nullptr;
    }
    if (!Get_Kek_By_Info)
    {
        cerr << "Error to read the function" << endl;
        dlclose(handle);
        return nullptr;
    }

    uint8_t* result;
    result = Get_Kek_By_Info(userId, info_len, kekInfo);
    return result;
}

uint8_t* Encrypt_data_key(long userId, uint8_t kekId, uint8_t key_length, uint8_t* arrRandom)
{
    typedef uint8_t* (*Encrypt_data_keyType)(long, uint8_t, uint8_t, uint8_t*);

    Encrypt_data_keyType Encrypt_data_key = (Encrypt_data_keyType)dlsym(handle, "Encrypt_data_key");
    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Error to read the function: " << dlsymError << std::endl;
        return nullptr;
    }
    if (!Encrypt_data_key)
    {
        cerr << "Error to read the function" << endl;
        dlclose(handle);
        return nullptr;
    }

    uint8_t* result;
    result = Encrypt_data_key(userId, kekId, key_length, arrRandom);
    return result;
}

uint8_t* Decrypt_data_key(long userId, uint8_t kekId, uint8_t encryptedKeyLength, uint8_t* encryptedKey) 
{
    typedef uint8_t* (*Decrypt_data_keyType)(long, uint8_t, uint8_t, uint8_t*);

    Decrypt_data_keyType Decrypt_data_key = (Decrypt_data_keyType)dlsym(handle, "Decrypt_data_key");
    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Error to read the function: " << dlsymError << std::endl;
        return nullptr;
    }
    if (!Decrypt_data_key)
    {
        cerr << "Error to read the function" << endl;
        dlclose(handle);
        return nullptr;
    }

    uint8_t* result;
    result = Decrypt_data_key(userId, kekId, encryptedKeyLength, enctyptedKey);
    return result;
}

#endif // __linux__
