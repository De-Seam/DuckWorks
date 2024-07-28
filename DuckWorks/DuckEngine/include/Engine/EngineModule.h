#pragma once
#include <Core/CoreModule.h>

class EngineModule : public CoreModule
{
public:
	EngineModule();
	virtual ~EngineModule() override;
	virtual void RegisterRTTI() override;
};

extern EngineModule* gEngineModule;