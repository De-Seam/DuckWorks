#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindow.h"

RTTI_VIRTUAL_CLASS_DEFINITION(DebugUIWindow)

Json DebugUIWindow::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mOpen);

	return json;
}

void DebugUIWindow::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mOpen);
}
