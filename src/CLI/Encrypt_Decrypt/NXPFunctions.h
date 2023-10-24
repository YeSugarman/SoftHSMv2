#pragma once
#include "INXPFunctions.h"

#ifdef __linux__
#include <dlfcn.h>
#include <stdint.h>
#include <iostream>

class NXPFunctions :
	public INXPFunctions
{
	//private:
	void* handle;
	INXPFunctions::sss_status_t status = INXPFunctions::sss_status_t::kStatus_SSS_Success;

public:
	NXPFunctions();
	~NXPFunctions();

	INXPFunctions::sss_status_t GetRandom(uint8_t* random, uint16_t size)override;
};
#endif // __linux__