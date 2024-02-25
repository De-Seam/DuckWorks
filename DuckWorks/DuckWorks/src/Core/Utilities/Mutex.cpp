#include "Precomp.h"
#include "Core/Utilities/Mutex.h"

void Mutex::ReadLock()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mConditionVariable.wait(lock, [this]() { return !mWriteLockRequestPending && !mWriteLocked; }); // Wait until there's no write lock
	mActiveReaders++;
}

void Mutex::ReadUnlock()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mActiveReaders--;
	gAssert(mActiveReaders >= 0, "Unlock called more often than lock!");
	if (mActiveReaders == 0)
	{
		mConditionVariable.notify_all(); // Notify one waiting writer, if any
	}
}

void Mutex::WriteLock()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mWriteLockRequestPending = true;
	mConditionVariable.wait(lock, [this]() { return mActiveReaders == 0 && !mWriteLocked; }); // Wait until no write lock and no readers
	mWriteLocked = true;
	mWriteLockRequestPending = false;
}

void Mutex::WriteUnlock()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mWriteLocked = false;
	mConditionVariable.notify_all(); // Notify all waiting readers and potentially a writer
}

bool Mutex::IsWriteLocked() const
{
	return mWriteLocked;
}

bool Mutex::IsReadLocked() const
{
	return mActiveReaders > 0;
}
