// Own includes
#include <Core/Utilities/TypeID.h>

// Core includes
#include <Core/Containers/HashMap.h>
#include <Core/Containers/String.h>
#include <Core/Threads/Mutex.h>
#include <Core/Utilities/Profiler.h>

int32 CORE_API gGetAndIncrementTypeID(const String& inTypeIDName)
{
	PROFILE_SCOPE(gGetAndIncrementTypeID)

	static HashMap<String, int32> mNextTypeIDMap;
	static UniqueMutex mNextTypeIDMapMutex;
	// Locking the mutex is an expensive operation, but these type IDs should only be registered during initialization
	// Or very occasionally
	ScopedUniqueMutexLock lock(mNextTypeIDMapMutex);
	int32& type_id = mNextTypeIDMap[inTypeIDName];
	return type_id++;
}
