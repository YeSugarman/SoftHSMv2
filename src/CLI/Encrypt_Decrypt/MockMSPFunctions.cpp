#include "MockMSPFunctions.h"

MockMSPFunctions::MockMSPFunctions()
{
}

MockMSPFunctions::~MockMSPFunctions()
{
}

void MockMSPFunctions::read(int magic, int command, int dataLenght, int* data, int crc)
{

}

void MockMSPFunctions::write(int* magic, int* status, int* dataLenght, int* result, int* crc)
{
	*status = IMSPFunctions::SUCCESS;

}
