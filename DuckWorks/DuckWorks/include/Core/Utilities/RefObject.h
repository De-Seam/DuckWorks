#pragma once
#include "RTTI.h"

class RefObject : public RTTIBaseClass
{
	RTTI_CLASS(RefObject, RTTIBaseClass)
private:
	Atomic<int32> mRefCount = 1;

	struct WeakRefCounter
	{
		bool mIsAlive = true;
		Atomic<int32> mRefCount = 1;
	};

	WeakRefCounter* mWeakRefCounter = nullptr;

	template<typename taRefClassType, typename... taRefClassArgs>
	friend class Ref;

	template<typename taRefClassType>
	friend class WeakRef;
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
		mPtr->mWeakRefCounter = new RefObject::WeakRefCounter();
	}

	Ref(const Ref& inOther)
	{
		gAssert(inOther->mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		++mPtr->mRefCount;
	}

	Ref(const WeakRef<taType>& inOther)
	{
		gAssert(inOther->mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		++mPtr->mRefCount;
	}

	~Ref()
	{
		gAssert(mPtr != nullptr, "The object was destroyed but a Ref was still held!");

		--mPtr->mRefCount;

		if (mPtr->mWeakRefCounter->mRefCount <= 0)
		{
			if (mPtr->mWeakRefCounter->mRefCount <= 0)
				delete mPtr->mWeakRefCounter;
			else
				mPtr->mWeakRefCounter->mIsAlive = false;

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
		++mWeakRefCounter->mRefCount;
	}

	WeakRef(const WeakRef<taType>& inWeakRef)
	{
		mPtr = inWeakRef.mPtr;
		++mWeakRefCounter->mRefCount;
	}

	~WeakRef()
	{
		--mWeakRefCounter->mRefCount;
		// If this was the last Ref and WeakRef referencing the ptr then we can destroy the all ref count
		if (mWeakRefCounter->mRefCount <= 0)
		{
			// Only destroy it if the object is already destroyed
			if (!mWeakRefCounter->mIsAlive)
				delete mWeakRefCounter;
		}
	}

	Ref<taType>* Get() const
	{
		if (mWeakRefCounter->mIsAlive)
			return Ref<taType>(*this);
		return nullptr;
	}

private:
	taType* mPtr = nullptr;
	RefObject::WeakRefCounter* mWeakRefCounter = nullptr;
};
