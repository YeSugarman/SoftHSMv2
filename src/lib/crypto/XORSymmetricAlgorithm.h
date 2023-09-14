#pragma once
#include <SymmetricAlgorithm.h>
class XORSymmetricAlgorithm : public SymmetricAlgorithm
{
public:
	//// The maximum bytes to encrypt/decrypt
	//BIGNUM* maximumBytes;
	//BIGNUM* counterBytes;

	XORSymmetricAlgorithm();

	// Inherited via SymmetricAlgorithm
	virtual bool wrapKey(const SymmetricKey* key, const SymWrap::Type mode, const ByteString& in, ByteString& out) override;
	virtual bool unwrapKey(const SymmetricKey* key, const SymWrap::Type mode, const ByteString& in, ByteString& out) override;
	virtual size_t getBlockSize() const override;
	virtual bool checkMaximumBytes(unsigned long bytes) override;

	virtual bool encryptInit(const SymmetricKey* key, const SymMode::Type mode = SymMode::CBC, const ByteString& IV = ByteString(), bool padding = true, size_t counterBits = 0, const ByteString& aad = ByteString(), size_t tagBytes = 0);
	virtual bool encryptUpdate(const ByteString& data, ByteString& encryptedData);
	virtual bool decryptInit(const SymmetricKey* key, const SymMode::Type mode = SymMode::CBC, const ByteString& IV = ByteString(), bool padding = true, size_t counterBits = 0, const ByteString& aad = ByteString(), size_t tagBytes = 0);
	virtual bool decryptUpdate(const ByteString& encryptedData, ByteString& data);

};