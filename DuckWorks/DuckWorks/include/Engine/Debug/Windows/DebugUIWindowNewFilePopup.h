#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowNewFilePopup : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowNewFilePopup, DebugUIWindow)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowNewFilePopup(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

	void SetPath(const String& inPath);

private:
	String mPath = "Assets/";
};

#endif
