#pragma once

// Core includes
#include "Core/Utilities/Types.h"

// Std includes
#include <mutex>
#include <condition_variable>

class Mutex
{
public:
	void ReadLock(); ///< Locks the mutex for reading
	void ReadUnlock(); ///< Unlocks the mutex from reading


	void WriteLock(); ///< Locks the mutex for writing
	void WriteUnlock(); ///< Unlocks the mutex from writing

	bool IsWriteLocked() const;
	bool IsReadLocked() const;

private:
	std::mutex mMutex;
	std::condition_variable mConditionVariable;
	int32 mActiveReaders = 0;
	bool mWriteLocked = false;
	bool mWriteLockRequestPending = false;
};

class BaseScopedMutexLock
{
protected:
	Mutex* mMutex = nullptr;
};

class ScopedMutexReadLock : public BaseScopedMutexLock
{
public:
	ScopedMutexReadLock(Mutex& inMutex)
	{
		mMutex = &inMutex;
		mMutex->ReadLock();
	}

	~ScopedMutexReadLock()
	{
		mMutex->ReadUnlock();
	}
};

class ScopedMutexWriteLock : public BaseScopedMutexLock
{
public:
	ScopedMutexWriteLock(Mutex& inMutex)
	{
		mMutex = &inMutex;
		mMutex->WriteLock();
	}

	~ScopedMutexWriteLock()
	{
		mMutex->WriteUnlock();
	}
};
