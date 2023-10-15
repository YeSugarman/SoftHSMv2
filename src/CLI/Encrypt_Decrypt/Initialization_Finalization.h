#pragma once

#pragma once

#include "GlobalInstance.h"


extern SoftHSM* hsm;


CK_SESSION_HANDLE InitSession(long slotNumber, char* password);
void CloseSession(CK_SESSION_HANDLE session);
