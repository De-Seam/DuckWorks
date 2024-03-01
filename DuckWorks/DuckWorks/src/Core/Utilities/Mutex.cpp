#include "Precomp.h"
#include "Core/Utilities/Mutex.h"

void Mutex::ReadLock()
{
	mMutex.lock_shared();
}

void Mutex::ReadUnlock()
{
	mMutex.unlock_shared();
}

void Mutex::WriteLock()
{
	mMutex.lock();
}

void Mutex::WriteUnlock()
{
	mMutex.unlock();
}
