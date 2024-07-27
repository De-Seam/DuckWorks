#pragma once
#include <utility>

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
	taType temp = std::move(ioA);
	ioA = std::move(ioB);
	ioB = std::move(temp);
}