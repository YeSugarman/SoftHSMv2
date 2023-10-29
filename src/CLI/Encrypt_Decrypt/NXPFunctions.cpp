#include "NXPFunctions.h"
#ifdef __linux__

NXPFunctions::NXPFunctions()
{
    handle = dlopen("./dumm_lib.so", RTLD_LAZY);
    if (!handle) {
        cerr << "can not load the library" << endl;
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

    getRandomType getRandom = (getRandomType)dlsym(handle, "get_randomPht");
    const char* dlsymError = dlerror();
    if (dlsymError) {
        std::cerr << "Error to read the function: " << dlsymError << std::endl;
        return INXPFunctions::sss_status_t::kStatus_SSS_Fail;
    }
    if (!getRandom)
    {
        cerr << "Error to read the function" << endl;
        dlclose(handle);
        return INXPFunctions::sss_status_t::kStatus_SSS_Fail;
    }
    status = getRandom(random, size);
    return status;
}

#endif // __linux__