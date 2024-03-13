#include "Precomp.h"
#include "Core/Utilities/Mutex.h"

#ifdef _DEBUG
THREADLOCAL Array<Mutex*> gMutexes;
#endif

void Mutex::ReadLock()
{
#ifdef _DEBUG
	gAssert(std::ranges::find(gMutexes.begin(), gMutexes.end(), this) == gMutexes.end(), "Mutex was already locked!");
	gMutexes.push_back(this);
#endif
	mMutex.lock_shared();
}

void Mutex::ReadUnlock()
{
#ifdef _DEBUG
	std::erase(gMutexes, this);
#endif
	mMutex.unlock_shared();
}

void Mutex::WriteLock()
{
#ifdef _DEBUG
	gAssert(std::ranges::find(gMutexes.begin(), gMutexes.end(), this) == gMutexes.end(), "Mutex was already locked!");
	gMutexes.push_back(this);
#endif
	mMutex.lock();
}

void Mutex::WriteUnlock()
{
#ifdef _DEBUG
	std::erase(gMutexes, this);
#endif
	mMutex.unlock();
}

ScopedMutexReadLock::ScopedMutexReadLock(Mutex& inMutex)
{
	mMutex = &inMutex;
	mMutex->ReadLock();
}

ScopedMutexReadLock::~ScopedMutexReadLock()
{
	mMutex->ReadUnlock();
}

ScopedMutexWriteLock::ScopedMutexWriteLock(Mutex& inMutex)
{
	mMutex = &inMutex;
	mMutex->WriteLock();
}

ScopedMutexWriteLock::~ScopedMutexWriteLock()
{
	mMutex->WriteUnlock();
}
