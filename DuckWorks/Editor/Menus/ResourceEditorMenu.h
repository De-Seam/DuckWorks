#pragma once
#include <Editor/Menus/EditorMenu.h>

class Resource;

class ResourceEditorMenu : public EditorMenu
{
	RTTI_CLASS(ResourceEditorMenu, EditorMenu)
public:
	ResourceEditorMenu(EditorApp& ioEditorApp, Resource& ioResource);

	virtual void Update(float inDeltaTime) override;

private:
	DC::Ref<Resource> mResource;
};
