#include "Precomp.h"
#include "Core/Utilities/Mutex.h"

#ifdef _DEBUG
struct MutexTracker
{
	bool IsLocked(Mutex* inMutex) const { return std::ranges::find(mMutexes.begin(), mMutexes.end(), inMutex) != mMutexes.end(); }
	bool IsLocked(UniqueMutex* inMutex) const { return std::ranges::find(mUniqueMutexes.begin(), mUniqueMutexes.end(), inMutex) != mUniqueMutexes.end(); }

	void AddMutex(Mutex* inMutex)
	{
		gAssert(!IsLocked(inMutex), "Mutex was already locked!");
		mMutexes.push_back(inMutex);
	}

	void AddMutex(UniqueMutex* inMutex)
	{
		gAssert(!IsLocked(inMutex), "Mutex was already locked!");
		mUniqueMutexes.push_back(inMutex);
	}

	void RemoveMutex(const Mutex* inMutex)
	{
		for (uint64 i = 0; i < mMutexes.size(); i++)
		{
			if (mMutexes[i] == inMutex)
			{
				mMutexes.erase(mMutexes.begin() + i);
				break;
			}
		}
	}

	void RemoveMutex(const UniqueMutex* inMutex)
	{
		for (uint64 i = 0; i < mUniqueMutexes.size(); i++)
		{
			if (mUniqueMutexes[i] == inMutex)
			{
				mUniqueMutexes.erase(mUniqueMutexes.begin() + i);
				break;
			}
		}
	}

	Array<Mutex*> mMutexes;
	Array<UniqueMutex*> mUniqueMutexes;
};

THREADLOCAL MutexTracker gMutexTracker;
#endif

void gInitMutexTracker()
{
#ifdef _DEBUG

#endif
}

void gDestroyMutexTracker() {}

Mutex::~Mutex()
{
#ifdef _DEBUG
	gAssert(!gMutexTracker.IsLocked(this), "Mutex was still locked!");
#endif
}

void Mutex::ReadLock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(Mutex::ReadLock)
#endif

#ifdef _DEBUG
	gMutexTracker.AddMutex(this);
#endif
	mMutex.lock_shared();
}

void Mutex::ReadUnlock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(Mutex::ReadUnlock)
#endif

#ifdef _DEBUG
	gMutexTracker.RemoveMutex(this);
#endif
	mMutex.unlock_shared();
}

bool Mutex::TryReadLock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(Mutex::TryReadLock)
#endif

	bool locked = mMutex.try_lock_shared();
#ifdef _DEBUG
	if (locked)
	{
		gMutexTracker.AddMutex(this);
	}
#endif
	return locked;
}

void Mutex::WriteLock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(Mutex::WriteLock)
#endif

#ifdef _DEBUG
	gMutexTracker.AddMutex(this);
#endif
	mMutex.lock();
}

void Mutex::WriteUnlock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(Mutex::ReadLock)
#endif

#ifdef _DEBUG
	gMutexTracker.RemoveMutex(this);
#endif
	mMutex.unlock();
}

UniqueMutex::~UniqueMutex()
{
#ifdef _DEBUG
	gAssert(!gMutexTracker.IsLocked(this), "Mutex was still locked!");
#endif
}

void UniqueMutex::Lock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(UniqueMutex::Lock)
#endif

#ifdef _DEBUG
	gMutexTracker.AddMutex(this);
#endif
	mMutex.lock();
}

void UniqueMutex::Unlock()
{
#ifdef PROFILE_MUTEXES
	PROFILE_SCOPE(UniqueMutex::Unlock)
#endif

#ifdef _DEBUG
	gMutexTracker.RemoveMutex(this);
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

ScopedUniqueMutexLock::ScopedUniqueMutexLock(UniqueMutex& inMutex)
{
	mMutex = &inMutex;
	mMutex->Lock();
}

ScopedUniqueMutexLock::~ScopedUniqueMutexLock()
{
	mMutex->Unlock();
}
