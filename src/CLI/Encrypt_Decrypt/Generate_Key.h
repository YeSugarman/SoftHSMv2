#include "GlobalInstance.h"

extern SoftHSM* hsm;
extern INXPFunctions* NXPProvider;

CK_ULONG gen_key(int key_length, CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey);
