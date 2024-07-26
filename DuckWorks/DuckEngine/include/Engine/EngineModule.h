#pragma once
#include <Renderer/RendererModule.h>

class EngineModule : public RendererModule
{
public:
	EngineModule();
	~EngineModule();
	virtual void RegisterRTTI() override;
};

THREADLOCAL extern EngineModule* gEngineModule;