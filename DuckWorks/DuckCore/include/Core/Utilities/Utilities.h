#pragma once
// Core includes
#include <Core/Utilities/TypeID.h>

// Std includes
#include <cstring>

template<typename taType>
taType gMin(taType inA, taType inB)
{
	return ((inA) < (inB) ? (inA) : (inB));
}

template<typename taType>
taType gMax(taType inA, taType inB)
{
	return ((inA) > (inB) ? (inA) : (inB));
}

template<typename taType>
taType gClamp(taType inX, taType inMin, taType inMax)
{
	return gMax(gMin(inX, inMax), inMin);
}

template<typename taType>
void gSwap(taType& ioA, taType& ioB)
{
	uint8 temp[sizeof(taType)];
	memcpy(temp, &ioA, sizeof(taType));
	memcpy(&ioA, &ioB, sizeof(taType));
	memcpy(&ioB, &temp, sizeof(taType));
}