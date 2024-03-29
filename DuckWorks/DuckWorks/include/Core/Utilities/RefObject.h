#pragma once
#include "RTTI.h"

class RefObject : public RTTIBaseClass
{
	RTTI_CLASS(RefObject, RTTIBaseClass)
private:
	Atomic<int32> mRefCount = 1;

	template<typename taRefClassType, typename... taRefClassArgs>
	friend class Ref;
};

template<typename taType, typename... taArgs>
class Ref
{
public:
	Ref(taArgs&&... inArgs)
	{
		static_assert(std::is_base_of_v<RefObject, taType>);
		mPtr = new taType(std::forward<taArgs>(inArgs)...);
	}

	Ref(const Ref& inOther)
	{
		mPtr = inOther.mPtr;
		mPtr->mRefCount++;
	}

	~Ref()
	{
		gAssert(mPtr != nullptr, "The object was destroyed but a Ref was still held!");
		mPtr->mRefCount--;
		if (mPtr->mRefCount <= 0)
		{
			delete mPtr;
		}
	}

	taType* Get() const { return mPtr; }
	taType* operator->() const { return mPtr; }

	template<typename taCastType>
	taCastType* Cast() const
	{
		return gCast<taCastType>(mPtr);
	}

private:
	taType* mPtr = nullptr;
};