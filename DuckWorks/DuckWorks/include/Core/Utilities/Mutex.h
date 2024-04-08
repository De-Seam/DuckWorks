#pragma once

// Core includes
#include "Core/Utilities/Types.h"
#include "Core/Utilities/Utilities.h"

// Std includes
#include <shared_mutex>

void gInitMutexTracker();
void gDestroyMutexTracker();

class Mutex
{
public:
	~Mutex();
	void ReadLock(); ///< Locks the mutex for reading
	void ReadUnlock(); ///< Unlocks the mutex from reading
	bool TryReadLock(); ///< Tries to read lock the mutex. Returns true if successfull

	void WriteLock(); ///< Locks the mutex for writing
	void WriteUnlock(); ///< Unlocks the mutex from writing

private:
	std::shared_mutex mMutex;
};

class BaseScopedMutexLock
{
protected:
	Mutex* mMutex = nullptr;
};

class ScopedMutexReadLock : public BaseScopedMutexLock
{
public:
	ScopedMutexReadLock(Mutex& inMutex);
	~ScopedMutexReadLock();
};

class ScopedMutexWriteLock : public BaseScopedMutexLock
{
public:
	ScopedMutexWriteLock(Mutex& inMutex);
	~ScopedMutexWriteLock();
};

template<typename taType>
struct MutexReadProtectedPtr
{
	MutexReadProtectedPtr(Mutex& inMutex, taType* inPtr, bool inAlreadyLocked = false) :
		mPtr(inPtr), mMutex(&inMutex)
	{
		if (!inAlreadyLocked)
			mMutex->ReadLock();
	}

	~MutexReadProtectedPtr()
	{
		if (mMutex)
			mMutex->ReadUnlock();
	}

	MutexReadProtectedPtr(const MutexReadProtectedPtr&) = delete;
	MutexReadProtectedPtr& operator=(const MutexReadProtectedPtr&) = delete;

	// Move constructor
	MutexReadProtectedPtr(MutexReadProtectedPtr&& inOther) noexcept :
		mPtr(inOther.mPtr), mMutex(inOther.mMutex)
	{
		inOther.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
	}

	// Move assignment operator
	MutexReadProtectedPtr& operator=(MutexReadProtectedPtr&& inOther) noexcept
	{
		if (this != &inOther)
		{
			// Safely unlock the current mutex if it exists
			if (mMutex)
			{
				mMutex->ReadUnlock();
			}

			// Transfer ownership
			mPtr = inOther.mPtr;
			mMutex = inOther.mMutex;
			inOther.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
		}
		return *this;
	}

	taType* operator->()
	{
		return mPtr;
	}

	const taType* operator->() const
	{
		return mPtr;
	}

	taType* Get()
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return mPtr;
	}

	const taType* Get() const
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return mPtr;
	}

	void Unlock()
	{
		gAssert(mMutex != nullptr, "The mutex was already unlocked!");
		mMutex->ReadUnlock();
		mMutex = nullptr;
	}

private:
	taType* mPtr = nullptr;
	Mutex* mMutex = nullptr;
};

template<typename taType>
struct MutexWriteProtectedPtr
{
	MutexWriteProtectedPtr(Mutex& inMutex, taType* inPtr, bool inAlreadyLocked = false) : mPtr(inPtr), mMutex(&inMutex)
	{
		if (!inAlreadyLocked)
			mMutex->WriteLock();
	}

	~MutexWriteProtectedPtr()
	{
		if (mMutex)
			mMutex->WriteUnlock();
	}

	MutexWriteProtectedPtr(const MutexWriteProtectedPtr&) = delete;
	MutexWriteProtectedPtr& operator=(const MutexWriteProtectedPtr&) = delete;

	// Move constructor
	MutexWriteProtectedPtr(MutexWriteProtectedPtr&& inOther) noexcept : mPtr(inOther.mPtr), mMutex(inOther.mMutex)
	{
		inOther.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
	}

	// Move assignment operator
	MutexWriteProtectedPtr& operator=(MutexWriteProtectedPtr&& inOther) noexcept
	{
		if (this != &inOther)
		{
			// Safely unlock the current mutex if it exists
			if (mMutex)
			{
				mMutex->WriteUnlock();
			}

			// Transfer ownership
			mPtr = inOther.mPtr;
			mMutex = inOther.mMutex;
			inOther.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
		}
		return *this;
	}

	taType* operator->() { return mPtr; }

	const taType* operator->() const { return mPtr; }

	taType* Get()
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return mPtr;
	}

	const taType* Get() const
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return mPtr;
	}

	void Unlock()
	{
		gAssert(mMutex != nullptr, "The mutex was already unlocked!");
		mMutex->WriteUnlock();
		mMutex = nullptr;
	}

private:
	taType* mPtr = nullptr;
	Mutex* mMutex = nullptr;
};
