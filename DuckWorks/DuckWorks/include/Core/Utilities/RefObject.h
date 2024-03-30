#pragma once
#include "RTTI.h"

class RefObject : public RTTIBaseClass
{
	RTTI_CLASS(RefObject, RTTIBaseClass)
private:
	struct RefCounter
	{
		Atomic<int32> mRefCount = 1;
		Atomic<int32> mWeakRefCount = 1;
	};
	RefCounter* mRefCounter = nullptr;
	
	template<typename taRefClassType, typename... taRefClassArgs>
	friend class Ref;
};

template<typename taType>
class WeakRef;

template<typename taType, typename... taArgs>
class Ref
{
public:
	Ref(taArgs&&... inArgs)
	{
		static_assert(std::is_base_of_v<RefObject, taType>);
		mPtr = new taType(std::forward<taArgs>(inArgs)...);
		mPtr->mRefCounter = new RefObject::RefCounter();
	}

	Ref(const Ref& inOther)
	{
		gAssert(inOther->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		mPtr->mWeakRefCount->mRefCount++;
	}

	Ref(const WeakRef& inOther)
	{
		gAssert(inOther.GetTotalRefCountPtr)
		gAssert(inPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		mPtr->mRefCount++;
	}

	~Ref()
	{
		gAssert(mPtr != nullptr, "The object was destroyed but a Ref was still held!");

		mPtr->mRefCounter->mRefCount--;
		mPtr->mRefCounter->mWeakRefCount--;

		if (mPtr->mRefCounter->mRefCount <= 0)
		{
			if(mPtr->mRefCounter->mWeakRefCount <= 0)
				delete mPtr->mRefCounter;

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

private:
};

template<typename taType>
class WeakRef
{
public:
	WeakRef(const Ref<taType>& inRef)
	{
		mPtr = inRef.mPtr;
		mPtr->(*mAllRefCount)++;
	}

	WeakRef(const WeakRef<taType>& inWeakRef)
	{
		mPtr = inWeakRef.mPtr;
		mPtr->(*mTotalRefCount)++;
	}

	~WeakRef()
	{
		mPtr->(*mAllRefCount)--;
		// If this was the last Ref and WeakRef referencing the ptr then we can destroy the all ref count
		if (*mAllRefCount <= 0)
		{
			delete mAllRefCount;
		}
	}

	Ref<taType>* Get() const 
	{ 
		if(*mTotalRefCount >= 0)
			return Ref<taType>(*this); 
		return nullptr;
	}

	Atomic<int32>* GetTotalRefCountPtr() const { return mAllRefCount; }
private:
	taType* mPtr = nullptr;
	RefObject::RefCounter* a = nullptr;
	Atomic<int32>* mTotalRefCount = nullptr;
};