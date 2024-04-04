#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

class DebugUIWindow : public RefObject
{
	RTTI_CLASS(DebugUIWindow, RefObject)

public:
	DebugUIWindow() = default;
	virtual ~DebugUIWindow() override = default;
	
	virtual void Update(float inDeltaTime) = 0;

	[[nodiscard]] virtual bool IsOpen() const { return mOpen; }
	bool* GetOpenPtr() { return &mOpen; }

protected:
	// The open variable should be given to ImGui::Begin() as the second parameter
	bool mOpen = true;
};
