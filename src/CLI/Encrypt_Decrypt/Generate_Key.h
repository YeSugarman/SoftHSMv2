#include "GlobalInstance.h"

extern SoftHSM* hsm;
extern INXPFunctions* NXPProvider;
extern IMSPFunctions* MSPProvider;

int gen_key(int key_length, CK_SLOT_ID slot, CK_SESSION_HANDLE session);