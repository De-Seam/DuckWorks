#pragma once
#include <Core/Utilities/Types.h>

//Random functions
inline uint32 gWangHash(uint32 inSeed)
{
	inSeed = (inSeed ^ 61) ^ (inSeed >> 16);
	inSeed *= 9;
	inSeed = inSeed ^ (inSeed >> 4);
	inSeed *= 0x27d4eb2d;
	inSeed = inSeed ^ (inSeed >> 15);
	return inSeed;
}

inline float gWangHashF(uint32 inSeed) // Return value between 0.f amd 1.f
{
	return gWangHash(inSeed) * (1.0f / 4294967296.0f);
}

inline uint32 gXorShift32(uint32* ioState)
{
	// Xorshift algorithm from George Marsaglia's paper
	uint32 state = *ioState;
	state ^= (state << 13);
	state ^= (state >> 17);
	state ^= (state << 5);
	return (*ioState) = state;
}

inline float gXorShift32F(uint32* ioState) // Return value between 0.f and 1.f
{
	return gXorShift32(ioState) * (1.0f / 4294967296.0f);
}
