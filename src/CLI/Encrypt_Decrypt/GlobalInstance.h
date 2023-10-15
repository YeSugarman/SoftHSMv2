#pragma once
#include "../../../src/lib/SoftHSM.h"

#include <fcntl.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>

#define FILL_ATTR(attr, typ, val, len) {(attr).type=(typ); (attr).pValue=(val); (attr).ulValueLen=len;}
#define S_IRUSR 0400
#define S_IWUSR 0200



// Create an instance of SoftHSM
SoftHSM* hsm = SoftHSM::i(); // Get the instance

