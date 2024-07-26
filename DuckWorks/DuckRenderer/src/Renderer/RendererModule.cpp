#include <Renderer/Precomp.h>
#include <Renderer/RendererModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// Renderer includes
#include <Renderer/Renderer.h>
#include <Renderer/WindowEvents/WindowEventManager.h>

THREADLOCAL RendererModule* gRendererModule = nullptr;

RendererModule::RendererModule()
{
	gAssert(gRendererModule == nullptr);
	gRendererModule = this;
}

RendererModule::~RendererModule()
{
	gAssert(gRendererModule != nullptr);
	gRendererModule = nullptr;
}

void RendererModule::RegisterRTTI()
{
	mRTTIFactory.RegisterClass<Renderer>();
	mRTTIFactory.RegisterClass<WindowEventManager>();
}
