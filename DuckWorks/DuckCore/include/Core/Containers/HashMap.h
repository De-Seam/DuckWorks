#pragma once

// External includes
#include <External/phmap/phmap.h>

template<typename taKeyType, typename taValueType>
class HashMap
{
public:
	void Add(const taKeyType& inKey, taValueType&& inItem) { mHashMap.emplace(inKey, inItem); }

	taValueType* Find(const taKeyType& inKey) 
	{ 
		auto iterator = mHashMap.find(inKey);
		if (iterator != mHashMap.end())
			return &((*iterator).second);
		return nullptr;
	}
	const taValueType* Find(const taKeyType& inKey) const 
	{
		auto iterator = mHashMap.find(inKey);
		if (iterator != mHashMap.end())
			return &((*iterator).second);
		return nullptr;
	}

	bool Contains(const taKeyType& inKey) const { return mHashMap.contains(inKey); }
	
	taValueType& operator[](const taKeyType& inKey) { return mHashMap[inKey]; }
	const taValueType& operator[](const taKeyType& inKey) const { return mHashMap[inKey]; }

private:
	phmap::flat_hash_map<taKeyType, taValueType> mHashMap;
};