
#include "GlobalInstance.h"


extern SoftHSM* hsm;

void Init();
void Final();

CK_SESSION_HANDLE InitSession(long slotNumber, char* password);
void CloseSession(CK_SESSION_HANDLE session);
