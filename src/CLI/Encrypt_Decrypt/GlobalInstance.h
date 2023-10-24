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
//#include <wiringPiI2C.h>


#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200

extern SoftHSM* hsm;


