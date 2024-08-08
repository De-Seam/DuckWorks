#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindow.h"

#ifdef _DEBUG

Json DebugUIWindow::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mOpen);

	return json;
}

void DebugUIWindow::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mOpen);
}

#endif
