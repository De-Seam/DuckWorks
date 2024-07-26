#pragma once
#include <Core/CoreModule.h>

class RendererModule : public CoreModule
{
public:
	RendererModule();
	~RendererModule();
	virtual void RegisterRTTI() override;
};

THREADLOCAL extern RendererModule* gRendererModule;