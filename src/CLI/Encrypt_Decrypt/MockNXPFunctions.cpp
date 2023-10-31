#include "MockNXPFunctions.h"
#include <stdlib.h>

MockNXPFunctions::MockNXPFunctions()
{
}

MockNXPFunctions::~MockNXPFunctions()
{
}

INXPFunctions::sss_status_t MockNXPFunctions::GetRandom(uint8_t* random, uint16_t size)
{
	int randNum = rand();
	*random = (uint8_t)randNum;
	return kStatus_SSS_Success;
}