#pragma once
#include <DuckCore/RTTI/RTTIClass.h>

struct ComponentBase : public DC::RTTIClass
{
	RTTI_CLASS(ComponentBase, RTTIClass)
public:
	virtual void UpdateImGui() {} // Update for ImGui, in the editor. This is already inside a TreeNode.
	virtual DC::Json ToJson() { return {}; }
	virtual void FromJson(const DC::Json& inJson) { (void)inJson; }
};
