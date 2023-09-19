#pragma once
#include "config.h"
#include "CryptoFactory.h"
#include "SymmetricAlgorithm.h"
#include "RNG.h"
#include <memory>

class XORCryptoFactory : public CryptoFactory
{
public:
	// Return the one-and-only instance
	static XORCryptoFactory* i();

	// This will destroy the one-and-only instance.
	static void reset();

	// Inherited via CryptoFactory
	virtual SymmetricAlgorithm* getSymmetricAlgorithm(SymAlgo::Type algorithm) override;
	virtual AsymmetricAlgorithm* getAsymmetricAlgorithm(AsymAlgo::Type algorithm) override;
	virtual HashAlgorithm* getHashAlgorithm(HashAlgo::Type algorithm) override;
	virtual MacAlgorithm* getMacAlgorithm(MacAlgo::Type algorithm) override;
	virtual RNG* getRNG(RNGImpl::Type name) override;

	static std::unique_ptr<XORCryptoFactory> instance;
};
