
#include "XORSymmetricAlgorithm.h"
#include <openssl/bn.h>
#include <openssl/evp.h>

bool XORSymmetricAlgorithm::wrapKey(const SymmetricKey* key, const SymWrap::Type mode, const ByteString& in, ByteString& out)
{
	return false;
}

bool XORSymmetricAlgorithm::unwrapKey(const SymmetricKey* key, const SymWrap::Type mode, const ByteString& in, ByteString& out)
{
	return false;
}

size_t XORSymmetricAlgorithm::getBlockSize() const
{
	return 128 >> 3;// as OSSLAES
}

bool XORSymmetricAlgorithm::checkMaximumBytes(unsigned long bytes)
{
	return true;//Based on the assumption that it can always be deciphered
}

bool XORSymmetricAlgorithm::encryptInit(const SymmetricKey* key, const SymMode::Type mode, const ByteString& IV, bool padding, size_t counterBits, const ByteString& aad, size_t tagBytes)
{
	// Call the superclass initialiser
	if (!SymmetricAlgorithm::encryptInit(key, mode, IV, padding, counterBits, aad, tagBytes))
	{
		return false;
	}

	// Check the IV
	if (mode != SymMode::GCM && (IV.size() > 0) && (IV.size() != getBlockSize()))
	{
		ERROR_MSG("Invalid IV size (%d bytes, expected %d bytes)", IV.size(), getBlockSize());

		ByteString dummy;
		SymmetricAlgorithm::encryptFinal(dummy);

		return false;
	}

	ByteString iv;

	if (IV.size() > 0)
	{
		iv = IV;
	}
	else
	{
		iv.wipe(getBlockSize());
	}

	return true;
}

bool XORSymmetricAlgorithm::encryptUpdate(const ByteString& data, ByteString& encryptedData)
{
	if (!SymmetricAlgorithm::encryptUpdate(data, encryptedData))
	{
		//clean();
		return false;
	}

	if (data.size() == 0)
	{
		encryptedData.resize(0);

		return true;
	}

	// Prepare the output block
	encryptedData.resize(data.size());

	int outLen = encryptedData.size();

	char xorKey = 'P';
	int len = data.size();

	for (int i = 0; i < len; i++)
	{
		encryptedData[i] = data.const_byte_str()[i] ^ xorKey;
	}

	// Resize the output block
	currentBufferSize -= outLen;

	return true;
}

bool XORSymmetricAlgorithm::decryptInit(const SymmetricKey* key, const SymMode::Type mode, const ByteString& IV, bool padding, size_t counterBits, const ByteString& aad, size_t tagBytes)
{
	// Call the superclass initialiser
	if (!SymmetricAlgorithm::decryptInit(key, mode, IV, padding, counterBits, aad, tagBytes))
	{
		return false;
	}

	// Check the IV
	if (mode != SymMode::GCM && (IV.size() > 0) && (IV.size() != getBlockSize()))
	{
		ERROR_MSG("Invalid IV size (%d bytes, expected %d bytes)", IV.size(), getBlockSize());

		ByteString dummy;
		SymmetricAlgorithm::decryptFinal(dummy);

		return false;
	}

	ByteString iv;

	if (IV.size() > 0)
	{
		iv = IV;
	}
	else
	{
		iv.wipe(getBlockSize());
	}

	return true;
}

bool XORSymmetricAlgorithm::decryptUpdate(const ByteString& encryptedData, ByteString& data)
{

	if (!SymmetricAlgorithm::decryptUpdate(encryptedData, data))
	{
		//clean();
		return false;
	}

	// Prepare the output block
	data.resize(encryptedData.size());

	int outLen = data.size();

	char xorKey = 'P';
	int len = encryptedData.size();
	int i = 0;
	while(encryptedData.const_byte_str()[i] != '\0')
	{
		data[i] = encryptedData.const_byte_str()[i++] ^ xorKey;
	}

	// Resize the output block
	currentBufferSize -= outLen;

	return true;
}

XORSymmetricAlgorithm::XORSymmetricAlgorithm() :SymmetricAlgorithm()
{
	/*maximumBytes = NULL;
	counterBytes = NULL;*/
}