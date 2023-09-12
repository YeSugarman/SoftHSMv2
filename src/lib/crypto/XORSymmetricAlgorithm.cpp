
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

	// Count number of bytes written
	/*if (maximumBytes)
	{
	BN_add_word(counterBytes, data.size());
	}*/

	// Prepare the output block
	encryptedData.resize(data.size());

	int outLen = encryptedData.size();

	char xorKey = 'P';
	int len = data.size();
	for (int i = 0; i < len; i++)
	{
		//data[i] ^ xorKey;
		encryptedData[i] = 'a';
	}

	//if (!EVP_EncryptUpdate(pCurCTX, &encryptedData[0], &outLen, (unsigned char*)data.const_byte_str(), data.size()))
	//{
	// ERROR_MSG("EVP_EncryptUpdate failed: %s", ERR_error_string(ERR_get_error(), NULL));

	// clean();

	// ByteString dummy;
	// SymmetricAlgorithm::encryptFinal(dummy);

	// return false;
	//}


	// Resize the output block
	encryptedData.resize(outLen);
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