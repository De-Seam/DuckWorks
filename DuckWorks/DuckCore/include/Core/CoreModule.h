#pragma once
#include <Core/Containers/SmartPointers.h>
#include <Core/Threads/Thread.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Modules are places to store global variables. They're accessible from each thread
// And they handle registering rtti types etc
class CoreModule
{
public:
	CoreModule();
	virtual ~CoreModule();
	virtual void RegisterRTTI();

	RTTIFactory mRTTIFactory;
private:
};

extern CoreModule* gCoreModule;