#include <Precomp.h>
#include <Core/CoreModule.h>

// Core includes
#include <Core/RTTI/RTTIRefObject.h>
#include <Core/Utilities/Assert.h>
#include <Core/Utilities/Profiler.h>

CoreModule* gCoreModule = nullptr;

CoreModule::CoreModule()
{
	gAssert(gCoreModule == nullptr);
	gCoreModule = this;
}

CoreModule::~CoreModule()
{
	gAssert(gCoreModule != nullptr);
	gCoreModule = nullptr;
}

void CoreModule::RegisterRTTI()
{
	PROFILE_SCOPE(CoreModule::RegisterRTTI)

	mRTTIFactory.RegisterClass<RTTIClass>();
	mRTTIFactory.RegisterClass<RTTIRefObject>();
}
