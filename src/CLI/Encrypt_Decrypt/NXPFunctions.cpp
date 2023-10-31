#include "NXPFunctions.h"
#ifdef __linux__

NXPFunctions::NXPFunctions()
{
    handle = dlopen("/home/pi/project/Yehudit_Symm/SoftHSMv2/src/CLI/Encrypt_Decrypt/dumm_lib.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "can not load the library" << std::endl;
        return;
    }
}

NXPFunctions::~NXPFunctions()
{
    dlclose(handle);
}

INXPFunctions::sss_status_t NXPFunctions::GetRandom(uint8_t* random, uint16_t size)
{
    typedef INXPFunctions::sss_status_t(*getRandomType)(uint8_t*, uint16_t);
    getRandomType getRandom = (getRandomType)dlsym(handle, "_Z10get_randomPht");
    //getRandomType getRandom = (getRandomType)dlsym(handle, "get_random");
    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Error to read the function: " << dlsymError << std::endl;
        return INXPFunctions::sss_status_t::kStatus_SSS_Fail;
    }
    if (!getRandom)
    {
        std::cerr << "Error to read the function" << std::endl;
        dlclose(handle);
        return INXPFunctions::sss_status_t::kStatus_SSS_Fail;
    }
    status = getRandom(random, size);
    return status;
}

#endif // __linux__