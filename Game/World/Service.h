#pragma once
#include <Engine/Objects/Object.h>

class Service : public Object
{
	RTTI_CLASS(Service, Object)
public:
	Service() = default;
	explicit Service(const DC::Json& aJson) : Base(aJson) {}
	virtual ~Service() override = default;
};

// World services are multithreaded.
class WorldService final : public Service
{
	RTTI_CLASS(WorldService, Service)
public:
	WorldService() = default;
	explicit WorldService(const DC::Json& aJson) : Base(aJson) {}
	virtual ~WorldService() override = default;
};

// Scene services are single threaded.
class SceneService final : public Service
{
	RTTI_CLASS(SceneService, Service)
public:
	SceneService() = default;
	explicit SceneService(const DC::Json& aJson) : Base(aJson) {}
	virtual ~SceneService() override = default;
};