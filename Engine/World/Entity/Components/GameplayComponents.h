#pragma once
#include <DuckCore/Math/Transform.h>
#include <DuckCore/RTTI/RTTIClass.h>

struct TransformComponent final : public DC::RTTIClass
{
	RTTI_CLASS(TransformComponent, RTTIClass)

	DC::Transform2D mTransform;

	void FromJson(const DC::Json& aJson);
	DC::Json ToJson() const;
};
