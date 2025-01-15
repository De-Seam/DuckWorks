#include <Editor/EditorSettings.h>
#include <Engine/Engine.h>
#include <Engine/Resources/ResourceManager.h>
#include <Engine/Renderer/TextureResource.h>
#include <Game/Entity/Components/RenderComponent.h>

using namespace DC;

void SpriteRenderComponent::OnCreatedByEditor()
{
	Ref<TextureResource> texture_resource = &gEngine->GetManager<ResourceManager>().Get<TextureResource>(EditorSettings::sGetDefaultTextureResourceGUID());
	mSprite = new Sprite(*texture_resource, { 0, 0 }, { 2, 2 });
}
