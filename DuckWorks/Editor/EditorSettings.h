#pragma once
#include <DuckCore/Utilities/GUID.h>

#include <Engine/Renderer/TextureResource.h>

class EditorSettings
{
public:
	DC::Json ToJson() const;
	void FromJson(const DC::Json& inJson);

	void SetDefaultTextureResource(TextureResource& inTextureResource) { mDefaultTextureResource = &inTextureResource; }
	const TextureResource* GetDefaultTextureResource() const { return mDefaultTextureResource; }
	TextureResource* GetDefaultTextureResource() { return mDefaultTextureResource; }

	static DC::GUID sGetDefaultTextureResourceGUID() { return DC::GUID(284732812); }

private:
	DC::Ref<TextureResource> mDefaultTextureResource;
};
