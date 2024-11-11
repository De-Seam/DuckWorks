#pragma once
// Core includes
#include <DuckCore/RTTI/Ref.h>

// Engine includes
#include <Engine/Manager.h>

class EngineUpdateHandle;

class SDLEventManager : public Manager
{
	RTTI_CLASS(SDLEventManager, Manager)
public:
	SDLEventManager();
	virtual ~SDLEventManager() override;

	void Update();

private:
	DC::Ref<EngineUpdateHandle> mUpdateHandle;
};
