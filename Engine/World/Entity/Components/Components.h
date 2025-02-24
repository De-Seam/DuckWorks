#pragma once
#include <DuckCore/Utilities/Json.h>

#include <Engine/World/Entity/ECSEntity.h>

#include <functional>

template<typename taType>
void RegisterComponentType();

class ECSEntity;

namespace Internal
{
void RegisterAddComponentFunction(std::function<ECSEntity&> aAddComponentFunction);
void RegisterRemoveComponentFunction(std::function<ECSEntity&> aRemoveComponentFunction);
void RegisterHasComponentFunction(std::function<bool(const ECSEntity&)> aHasComponentFunction);

void RegisterComponentFromJsonFunction(std::function<void(ECSEntity&, const DC::Json&)> aFromJsonFunction);
void RegisterComponentToJsonFunction(std::function<DC::Json(const ECSEntity&)> aToJsonFunction);

}

template<typename taType>
void RegisterComponentType()
{
	Internal::RegisterAddComponentFunction([](ECSEntity& aEntity) { aEntity.AddComponent<taType>(); });
	Internal::RegisterRemoveComponentFunction([](ECSEntity& aEntity) { aEntity.RemoveComponent<taType>(); });
	Internal::RegisterHasComponentFunction([](const ECSEntity& aEntity) { return aEntity.HasComponent<taType>(); });

	Internal::RegisterComponentFromJsonFunction([](ECSEntity& aEntity, const DC::Json& aJson) { aEntity});
}
