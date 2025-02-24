#include <Engine/World/Entity/Components/GameplayComponents.h>

using namespace DC;

void TransformComponent::FromJson(const Json& aJson)
{
	mTransform = aJson;
}

Json TransformComponent::ToJson() const
{
	Json json = mTransform;
	return json;
}
