#include "GlobalInstance.h"

#ifdef __linux__
	INXPFunctions* NXPProvider = new NXPFunctions();
#endif
	INXPFunctions* NXPProvider = new MockNXPFunctions();

SoftHSM* hsm = SoftHSM::i(); // Get the instance