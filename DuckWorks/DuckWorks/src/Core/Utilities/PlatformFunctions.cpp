#include "Core/Utilities/PlatformFunctions.h"

// Core includes
#include "Core/Utilities/Types.h"

#ifdef _WIN32

// Windows.h
#include <Windows.h>

// Std includes
#include <psapi.h>

uint64 gGetMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX memCounter;
	if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memCounter), sizeof(memCounter)))
		return memCounter.WorkingSetSize;

	return 0;
}

#endif // _WIN32
