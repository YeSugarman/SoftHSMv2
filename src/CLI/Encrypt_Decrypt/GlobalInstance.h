#pragma once
#include "SoftHSM.h"

#include <fcntl.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdint>
#include "INXPFunctions.h"
#include "MockNXPFunctions.h"
#include "NXPFunctions.h"
#include "IMSPFunctions.h"
#include "MockMSPFunctions.h"
#include "MSPFunctions.h"


#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200
#define MAX_LENGTH 32


extern INXPFunctions* NXPProvider;
extern IMSPFunctions* MSPProvider;

extern SoftHSM* hsm;

unsigned int callCrc(unsigned int* rxBuffer, int length);
unsigned int CCITT_Update(unsigned int init, unsigned int input);
