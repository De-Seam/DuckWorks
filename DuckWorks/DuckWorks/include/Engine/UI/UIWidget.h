#pragma once
#include "Core/RTTI/RTTI.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

class UIWidget : public RefObject
{
	RTTI_CLASS(UIWidget, RefObject, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		UIWidget* mParent = nullptr;
	};

	UIWidget(const ConstructParameters& inParameters = {}) : Base(inParameters) {}

	virtual void Update(float inDeltaTime);
	virtual void Draw();

	const AABB& GetAABB() const { return mAABB; }

protected:
	virtual void CalculateAABB();

private:
	Ref<UIWidget> mParent = nullptr;
	Array<Ref<UIWidget>> mChildren;

	AABB mAABB;
};
