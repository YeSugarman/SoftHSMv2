#include "GlobalInstance.h"

#ifdef __linux__
	INXPFunctions* NXPProvider = new NXPFunctions();
#else
	INXPFunctions* NXPProvider = new MockNXPFunctions();
#endif

SoftHSM* hsm = SoftHSM::i(); // Get the instance