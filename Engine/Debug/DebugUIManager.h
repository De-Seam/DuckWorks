#pragma once
#include <DuckCore/Managers/Manager.h>

#include <Engine/Debug/DebugWindow.h>

class DebugUIManager : public DC::Manager
{
	MANAGER_BASE_CLASS(DebugUIManager)
public:
	

private:
	DC::Array<DC::Ref<DebugWindow>> mRootWindows;
};
