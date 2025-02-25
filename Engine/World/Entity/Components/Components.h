#pragma once
#include <DuckCore/Core/Log.h>
#include <DuckCore/RTTI/RTTI.h>
#include <DuckCore/Utilities/Json.h>

#include <Engine/World/Entity/ECSEntity.h>

#include <functional>

struct Component;

namespace DC {
class String;
}

struct LogCategoryComponents final : public DC::LogCategory
{
	RTTI_CLASS(LogCategoryComponents, LogCategory)
};

template<typename taType>
void RegisterComponentType();

class ECSEntity;

const DC::RTTI* FindComponentRTTIByName(const DC::String& aName);
Component* AddComponent(ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);
void RemoveComponent(ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);
Component* FindComponent(ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);
const Component* FindComponent(const ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);
bool HasComponent(const ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);
void FromJson(ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI, const DC::Json& aJson);
DC::Json ToJson(const ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI);

void GetAllComponentRTTIs(DC::Array<const DC::RTTI*> outComponentRTTIs);

namespace Internal
{
void RegisterComponentFunctions(
	const DC::RTTI& aComponentRTTI, 
	std::function<Component*(ECSEntity&)> aAddComponentFunction, 
	std::function<void(ECSEntity&)> aRemoveComponentFunction,
	std::function<Component*(ECSEntity&)> aFindComponentFunction,
	std::function<const Component*(const ECSEntity&)> aFindConstComponentFunction,
	std::function<bool(const ECSEntity&)> aHasComponentFunction,
	std::function<void(ECSEntity&, const DC::Json&)> aFromJsonFunction,
	std::function<DC::Json(const ECSEntity&)> aToJsonFunction);

}

template<typename taType>
void RegisterComponentType()
{
	Internal::RegisterComponentFunctions(
		taType::sGetRTTI(),
		[](ECSEntity& aEntity) { return &aEntity.AddComponent<taType>(); },
		[](ECSEntity& aEntity) { aEntity.RemoveComponent<taType>(); },
		[](ECSEntity& aEntity) { aEntity.FindComponent<taType>(); },
		[](const ECSEntity& aEntity) { return aEntity.HasComponent<taType>(); },
		[](ECSEntity& aEntity, const DC::Json& aJson) { aEntity.GetComponent<taType>().FromJson(aJson); },
		[](ECSEntity& aEntity) { return aEntity.GetComponent<taType>().ToJson(); });
}
