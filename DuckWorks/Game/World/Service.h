#pragma once
#include <Engine/Objects/Object.h>

class World;

class Service : public Object
{
	RTTI_CLASS(Service, Object)
public:
	Service(World& inWorld) : mWorld(&inWorld) {}

	World& GetWorld() { return *mWorld; }
	const World& GetWorld() const { return *mWorld; }

private:
	World* mWorld = nullptr;
};