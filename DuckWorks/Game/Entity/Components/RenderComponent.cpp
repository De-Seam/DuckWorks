#include <Editor/EditorSettings.h>
#include <Editor/Utilities/ImGuiHelper.h>

#include <Engine/Engine.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/ResourceManager.h>

#include <Game/Entity/Components/RenderComponent.h>

#include <imgui/imgui.h>

using namespace DC;

void SpriteRenderComponent::OnCreatedByEditor()
{
	Ref<TextureResource> texture_resource = &gEngine->GetManager<ResourceManager>().Get<TextureResource>(EditorSettings::sGetDefaultTextureResourceGUID());
	mSprite = new Sprite(*texture_resource, { 0, 0 }, { 2, 2 });
}

void SpriteRenderComponent::UpdateImGui(EditorMenu& ioEditorMenu)
{
	ImGui::PushID("TransformComponent");

	Ref<Resource> texture_resource = nullptr;
	ImGuiHelpers::sDrawResourceLink("Texture", TextureResource::sGetRTTI(), texture_resource, ioEditorMenu);

	if (texture_resource != nullptr)
		mSprite = new Sprite(texture_resource->Cast<TextureResource>(), { 0, 0 }, { 128, 128 });

	ImGui::PopID();
}
