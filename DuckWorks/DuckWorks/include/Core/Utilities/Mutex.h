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
struct MutexReadProtectedValue
{
	MutexReadProtectedValue(Mutex& inMutex, taType& inValue, bool inAlreadyLocked = false) :
		mValue(inValue), mMutex(&inMutex)
	{
		if (!inAlreadyLocked)
			mMutex->ReadLock();
	}

	~MutexReadProtectedValue()
	{
		if (mMutex)
			mMutex->ReadUnlock();
	}

	MutexReadProtectedValue(const MutexReadProtectedValue&) = delete;
	MutexReadProtectedValue& operator=(const MutexReadProtectedValue&) = delete;

	// Move constructor
	MutexReadProtectedValue(MutexReadProtectedValue&& other) noexcept :
		mValue(other.mValue), mMutex(other.mMutex)
	{
		other.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
	}

	// Move assignment operator
	MutexReadProtectedValue& operator=(MutexReadProtectedValue&& other) noexcept
	{
		if (this != &other)
		{
			// Safely unlock the current mutex if it exists
			if (mMutex)
			{
				mMutex->ReadUnlock();
			}

			// Transfer ownership
			mValue = other.mValue;
			mMutex = other.mMutex;
			other.mMutex = nullptr; // Prevent the destructor of 'other' from unlocking the mutex
		}
		return *this;
	}

	taType* operator->()
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return &mValue;
	}

	const taType* operator->() const
	{
		gAssert(mMutex != nullptr, "The mutex was unlocked, access to the object denied!");
		return &mValue;
	}

	void Unlock()
	{
		gAssert(mMutex != nullptr, "The mutex was already unlocked!");
		mMutex->ReadUnlock();
		mMutex = nullptr;
	}

private:
	taType& mValue;
	Mutex* mMutex = nullptr;
};
