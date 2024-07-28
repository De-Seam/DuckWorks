#pragma once
#include <Engine/EngineModule.h>

class AppModule : public EngineModule
{
public:
	AppModule();
	virtual ~AppModule();
	virtual void RegisterRTTI() override;
};

extern AppModule* gAppModule;