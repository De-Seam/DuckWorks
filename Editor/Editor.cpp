#include <Editor/Editor.h>

using namespace DC;

Editor::Editor() = default;

Editor::Editor(const Json& aJson) :
	Base(aJson)
{
	
}

Editor::~Editor() = default;

Json Editor::ToJson() const
{
	return Base::ToJson();
}
