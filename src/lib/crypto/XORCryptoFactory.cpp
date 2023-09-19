#include "XORCryptoFactory.h"

XORCryptoFactory* XORCryptoFactory::i()
{
	if (!instance.get())
	{
		instance.reset(new XORCryptoFactory());
	}

	return instance.get();
}

void XORCryptoFactory::reset()
{
	instance.reset();
}

SymmetricAlgorithm* XORCryptoFactory::getSymmetricAlgorithm(SymAlgo::Type algorithm)
{
	return nullptr;
}

AsymmetricAlgorithm* XORCryptoFactory::getAsymmetricAlgorithm(AsymAlgo::Type algorithm)
{
	return nullptr;
}

HashAlgorithm* XORCryptoFactory::getHashAlgorithm(HashAlgo::Type algorithm)
{
	return nullptr;
}

MacAlgorithm* XORCryptoFactory::getMacAlgorithm(MacAlgo::Type algorithm)
{
	return nullptr;
}

RNG* XORCryptoFactory::getRNG(RNGImpl::Type name)
{
	return nullptr;
}
