#include "GlobalInstance.h"

#ifdef __linux__
	INXPFunctions* NXPProvider = new NXPFunctions();
 	IMSPFunctions* MSPProvider = new MSPFunctions();
#else
	INXPFunctions* NXPProvider = new MockNXPFunctions();
	IMSPFunctions* MSPProvider = new MockMSPFunctions();
#endif


SoftHSM* hsm = SoftHSM::i(); // Get the instance

unsigned int callCrc(unsigned int* rxBuffer, int length)
{
	unsigned int SW_Results = 0xffff;  // Init SW CRC
	int i;
	for (i = 0; i < length; i++)
	{
		// First input lower byte
		SW_Results = CCITT_Update(SW_Results, rxBuffer[i] & 0xFF);
		// Then input upper byte
		SW_Results = CCITT_Update(SW_Results, (rxBuffer[i] >> 8) & 0xFF);
	}
	return SW_Results;
}


unsigned int CCITT_Update(unsigned int init, unsigned int input)
{
	unsigned int CCITT = (unsigned char)(init >> 8) | (init << 8);
	CCITT ^= input;
	CCITT ^= (unsigned char)(CCITT & 0xFF) >> 4;
	CCITT ^= (CCITT << 8) << 4;
	CCITT ^= ((CCITT & 0xFF) << 4) << 1;
	return CCITT;
}