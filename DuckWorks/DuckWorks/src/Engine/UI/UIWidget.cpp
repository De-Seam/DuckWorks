#include "Precomp.h"
#include "Engine/UI/UIWidget.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Factory/Factory.h"

RTTI_CLASS_DEFINITION(UIWidget, StandardAllocator)

Json UIWidget::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mChildren);

	return json;
}

void UIWidget::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
	if (inJson.contains("mChildren"))
	{
		const Json& children_json = inJson["mChildren"];
		UIWidget::ConstructParameters construct_params;
		construct_params.mParent = this;
		for (const Json& child : children_json)
			mChildren.push_back(gUIWidgetFactory.CreateClassFromJsonAndDeserialize(child, construct_params));
	}
}

void UIWidget::Update(float inDeltaTime)
{
	PROFILE_SCOPE(UIWidget::Update)

	for (UIWidget* child : mChildren)
		child->Update(inDeltaTime);
}

void UIWidget::Draw()
{
	PROFILE_SCOPE(UIWidget::Draw)

	for (UIWidget* child : mChildren)
		child->Draw();
}

void UIWidget::CalculateAABB()
{
	mAABB = {};
	for (UIWidget* child : mChildren)
	{
		child->CalculateAABB();
		mAABB = gComputeEncompassingAABB(mAABB, child->GetAABB());
	}
}
