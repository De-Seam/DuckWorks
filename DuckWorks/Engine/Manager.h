#pragma once
#include <DuckCore/RTTI/RTTIClass.h>

// Managers are the core of the engine. They are responsible for managing the engine's subsystems.
// Managers are singletons and are created by the engine.
class Manager : public DC::RTTIClass
{
	RTTI_VIRTUAL_CLASS(Manager, RTTIClass)
public:
	virtual void Init() {}
	virtual void Shutdown() {}
};