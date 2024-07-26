#pragma once
#include <Core/Containers/SmartPointers.h>
#include <Core/Threads/Thread.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Modules are places to store local variables. They're accessible from the main thread of each application
// And they handle registering rtti types etc
class CoreModule
{
public:
	CoreModule();
	~CoreModule();
	virtual void RegisterRTTI();

	RTTIFactory mRTTIFactory;
private:
};

THREADLOCAL extern CoreModule* gCoreModule;