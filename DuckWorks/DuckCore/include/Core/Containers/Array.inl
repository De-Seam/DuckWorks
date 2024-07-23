#pragma once
#include <Core/Utilities/Assert.h>

template<typename taType>
template<class ... taArgs>
void Array<taType>::Add(taArgs&&... inArgs)
{
	mArray.emplace_back(std::forward<taArgs>(inArgs)...);
}

template<typename taType>
bool Array<taType>::IsValidIndex(int32 inIndex) const
{
	return inIndex >= 0 && inIndex < mArray.size();
}

template<typename taType>
taType& Array<taType>::operator[](int32 inIndex)
{
	gAssert(IsValidIndex(inIndex), "");
	return mArray[inIndex];
}
