#pragma once
#include <Core/CoreModule.h>

class EngineModule : public CoreModule
{
public:
	EngineModule();
	~EngineModule();
	virtual void RegisterRTTI() override;
};

THREADLOCAL extern EngineModule* gEngineModule;