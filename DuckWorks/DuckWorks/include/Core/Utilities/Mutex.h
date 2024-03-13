#pragma once

// Core includes
#include "Core/Utilities/Types.h"
#include "Core/Utilities/Utilities.h"

// Std includes
#include <shared_mutex>

class Mutex
{
public:
	void ReadLock(); ///< Locks the mutex for reading
	void ReadUnlock(); ///< Unlocks the mutex from reading


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
