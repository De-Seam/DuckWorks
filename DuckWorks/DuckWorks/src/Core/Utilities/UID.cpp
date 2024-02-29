#include "Precomp.h"
#include "Core/Utilities/UID.h"
#include <mutex>

uint64 gLastUID = 0;
Mutex gMutex = {};

UID::UID()
{
	ScopedMutexWriteLock lock(gMutex);
	
	gLastUID++;
	mUID = gLastUID;
}