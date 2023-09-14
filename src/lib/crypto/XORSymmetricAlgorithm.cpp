
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
	return size_t();
}

bool XORSymmetricAlgorithm::checkMaximumBytes(unsigned long bytes)
{
	return false;
}

bool XORSymmetricAlgorithm::encryptUpdate(const ByteString& data, ByteString& encryptedData)
{
	if (!SymmetricAlgorithm::encryptUpdate(data, encryptedData))
	{
		clean();
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

bool XORSymmetricAlgorithm::decryptUpdate(const ByteString& encryptedData, ByteString& data)
{

	if (!SymmetricAlgorithm::decryptUpdate(encryptedData, data))
	{
		clean();
		return false;
	}

	// Prepare the output block
	data.resize(encryptedData.size());

	int outLen = data.size();

	char xorKey = 'P';
	int len = data.size();
	for (int i = 0; i < len; i++)
	{
		data[i] = encryptedData.const_byte_str()[i] ^ xorKey;
	}

	// Resize the output block
	currentBufferSize -= outLen;

	return true;
}

void XORSymmetricAlgorithm::clean()
{
	/*BN_free(maximumBytes);
	maximumBytes = NULL;
	BN_free(counterBytes);
	counterBytes = NULL;*/
}

XORSymmetricAlgorithm::XORSymmetricAlgorithm() :SymmetricAlgorithm()
{
	/*maximumBytes = NULL;
	counterBytes = NULL;*/
}