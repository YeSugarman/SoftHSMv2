#include "MSPFunctions.h"
#ifdef __linux__

MSPFunctions::MSPFunctions()
{
    handle = dlopen("./....so", RTLD_LAZY);
    if (!handle) {
        cerr << "can not load the library" << endl;
        return;
    }
}

MSPFunctions::~MSPFunctions()
{
    dlclose(handle);
}

uint8_t* MSPFunctions::Get_Kek_By_Info(int dataLength, int* data, int crc)
{
    typedef uint8_t*(*Get_Kek_By_InfoType)(int,int*,int);

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
    result = Get_Kek_By_Info(dataLength, data, crc);
    return result;
}

uint8_t* MSPFunctions::Encrypt_data_key(int dataLength, int* data, int crc)
{
    typedef uint8_t* (*Encrypt_data_keyType)(int, int*, int);

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
    result = Encrypt_data_key(dataLength, data, crc);
    return result;
}

uint8_t* MSPFunctions::Decrypt_data_key(int dataLength, int* data, int crc)
{
    typedef uint8_t* (*Decrypt_data_keyType)(int, int*, int);

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
    result = Decrypt_data_key(dataLength, data, crc);
    return result;
}

#endif // __linux__
