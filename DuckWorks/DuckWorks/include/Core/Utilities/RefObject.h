#pragma once
#include "RTTI.h"

class RefObject : public RTTIBaseClass
{
	RTTI_CLASS(RefObject, RTTIBaseClass)
public:
	RefObject() = default;
	virtual ~RefObject() override = default;

private:
	Atomic<int32> mRefCount = 0;

	struct WeakRefCounter
	{
		bool mIsAlive = true;
		Atomic<int32> mRefCount = 0;
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
		mPtr->mRefCount++;
	}

	Ref(const Ref<taType>& inOther)
	{
		gAssert(inOther.mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		mPtr->mRefCount++;
	}

	Ref<taType>& operator=(const Ref<taType>& inOther)
	{
		gAssert(mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr->mRefCount++;
		return *this;
	}

	//TODO: Move operator

	// Construct a ref from self
	Ref(taType* inSelf)
	{
		static_assert(std::is_base_of_v<RefObject, taType>);
		mPtr = inSelf;
		mPtr->mRefCount++;
		if(mPtr->mWeakRefCounter == nullptr)
			mPtr->mWeakRefCounter = new RefObject::WeakRefCounter();
	}

	template<typename taParentClass>
	Ref(const Ref<taParentClass>& inOther)
	{
		static_assert(std::is_base_of_v<taParentClass, taType> || std::is_base_of_v<taType, taParentClass>);
		gAssert(inOther.mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = SCast<taType*>(inOther.mPtr);
		mPtr->mRefCount++;
	}

	~Ref()
	{
		gAssert(mPtr != nullptr, "The object was destroyed but a Ref was still held!");

		mPtr->mRefCount--;

		if (mPtr->mRefCount <= 0)
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

	bool operator==(const Ref<taType>& inOther) const { return mPtr == inOther.mPtr; }

	template<typename taCastType>
	taCastType* Cast() const
	{
		return gCast<taCastType>(mPtr);
	}

	bool IsValid() const { return mPtr != nullptr; }

private:
	taType* mPtr = nullptr;

private:
	// Private so only WeakRef can use it
	Ref(const WeakRef<taType>& inOther)
	{
		// Empty ref
		if (!inOther.mWeakRefCounter->mIsAlive)
			return;

		gAssert(inOther.mPtr->mRefCount > 0, "Ref object is already destroyed!");
		mPtr = inOther.mPtr;
		++mPtr->mRefCount;
	}

	template<typename taRefClassType, typename... taRefClassArgs>
	friend class Ref;

	template<typename taRefClassType>
	friend class WeakRef;
};

template<typename taType>
class WeakRef
{
public:
	WeakRef(const Ref<taType>& inRef)
	{
		mPtr = inRef.mPtr;
		mWeakRefCounter = mPtr->mWeakRefCounter;
		mWeakRefCounter->mRefCount++;
	}

	WeakRef(const WeakRef<taType>& inWeakRef)
	{
		mPtr = inWeakRef.mPtr;
		mWeakRefCounter = inWeakRef.mWeakRefCounter;
		mWeakRefCounter->mRefCount++;
	}

	~WeakRef()
	{
		mWeakRefCounter->mRefCount--;
		// If this was the last Ref and WeakRef referencing the ptr then we can destroy the all ref count
		if (mWeakRefCounter->mRefCount <= 0)
		{
			// Only destroy it if the object is already destroyed
			if (!mWeakRefCounter->mIsAlive)
				delete mWeakRefCounter;
		}
	}

	Ref<taType> Get() const
	{
		return Ref<taType>(*this);
	}

	bool IsAlive() const { return mWeakRefCounter->mIsAlive; }

	bool operator==(const Ref<taType>& inOther) const { return mPtr == inOther.mPtr; }

private:
	taType* mPtr = nullptr;
	RefObject::WeakRefCounter* mWeakRefCounter = nullptr;

	template<typename taRefClassType, typename... taRefClassArgs>
	friend class Ref;
};
