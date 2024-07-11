#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowTextureViewer : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowTextureViewer, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowTextureViewer(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	void SetTexture(const SharedPtr<TextureResource>& inTexture) { mTexture = inTexture; }

	virtual void Update(float inDeltaTime) override;

private:
	SharedPtr<TextureResource> mTexture = nullptr;

	float mTimeSinceClipboardCopied = 1000.f;
	float mCopiedToClipboardTime = 1.f;
};

#endif
