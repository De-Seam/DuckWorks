#pragma once
// Core includes
#include <Core/Utilities/Types.h>

// Std includes
#include <vector>

template<typename taType>
class Array
{
public:
	template<class... taArgs>
	void Add(taArgs&&... inArgs);

	bool IsValidIndex(int32 inIndex) const;

	taType& operator[](int32 inIndex);

private:
	std::vector<taType> mArray;
};

#include "Array.inl"
