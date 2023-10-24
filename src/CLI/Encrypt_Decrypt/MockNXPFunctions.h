#pragma once
#include "INXPFunctions.h"

class MockNXPFunctions :
	public INXPFunctions
{
public:
	MockNXPFunctions();
	~MockNXPFunctions();
	
	virtual sss_status_t GetRandom(uint8_t* random, uint16_t size) override;
};