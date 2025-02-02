#include "Precomp.h"
#include "Core/Utilities/UID.h"
#include <mutex>

Atomic<uint64> gLastUID = 0;
Mutex gMutex = {};

UID::UID()
{
	gLastUID++;
	mUID = gLastUID;
}
