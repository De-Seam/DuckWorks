#pragma once
#include <DuckCore/Math/Transform.h>

#include <Engine/World/Entity/Components/Component.h>

struct TransformComponent final : public Component
{
	RTTI_CLASS(TransformComponent, Component)
public:
	DC::Transform2D mTransform;

	virtual void FromJson(const DC::Json& aJson) override;
	virtual DC::Json ToJson() const override;
};
