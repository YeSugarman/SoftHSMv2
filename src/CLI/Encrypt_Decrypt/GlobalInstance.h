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


#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200
#define MAX_LENGTH 32


extern INXPFunctions* NXPProvider;
extern SoftHSM* hsm;


