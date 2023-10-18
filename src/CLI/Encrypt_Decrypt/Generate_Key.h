#pragma once

#include "GlobalInstance.h"


extern SoftHSM* hsm;

int gen_key(CK_SLOT_ID slot, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE* hSecretKey);