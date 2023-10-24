#pragma once
#include <cstdint>

class INXPFunctions
{
public:

	typedef enum
	{
		/** Operation was successful */
		kStatus_SSS_Success = 0x5a5a5a5au,
		/** Operation failed */
		kStatus_SSS_Fail = 0x3c3c0000u,
		/** Operation not performed because some of the passed parameters
		 * were found inappropriate */
		kStatus_SSS_InvalidArgument = 0x3c3c0001u,
		// LCOV_EXCL_START
		/** Where the underlying sub-system supports multi-threading,
		 * Internal status to handle simultaneous access.
		 *
		 * This status is not expected to be returned to higher layers.
		 * */
		kStatus_SSS_ResourceBusy = 0x3c3c0002u,
		// LCOV_EXCL_STOP
	} sss_status_t;

	virtual sss_status_t GetRandom(uint8_t* random, uint16_t size) = 0;
};