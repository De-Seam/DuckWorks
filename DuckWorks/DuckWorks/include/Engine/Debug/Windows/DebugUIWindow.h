#pragma once
#include "Core/CoreBase.h"
#include "Core/RTTI/RTTI.h"

class DebugUIWindow : public RefObject
{
	RTTI_VIRTUAL_CLASS(DebugUIWindow, RefObject)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindow(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}
	virtual ~DebugUIWindow() override = default;

	// Multi threaded update for performance heavy task
	virtual void UpdateMultiThreaded(float inDeltaTime) { (void)inDeltaTime; }
	// Single threaded update for the actual ImGui drawing
	virtual void Update(float inDeltaTime) = 0;

	[[nodiscard]] virtual bool IsOpen() const { return mOpen; }
	bool* GetOpenPtr() { return &mOpen; }

protected:
	// The open variable should be given to ImGui::Begin() as the second parameter
	bool mOpen = true;

	friend class DebugUIWindowManager;
};
