#include "GlobalInstance.h"

extern SoftHSM* hsm;
extern INXPFunctions* NXPProvider;

void gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey);
