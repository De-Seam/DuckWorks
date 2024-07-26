#pragma once
#include <Engine/EngineModule.h>

class AppModule : public EngineModule
{
public:
	AppModule();
	~AppModule();
	virtual void RegisterRTTI() override;
};

THREADLOCAL extern AppModule* gAppModule;