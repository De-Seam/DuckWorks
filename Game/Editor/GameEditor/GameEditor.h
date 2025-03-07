#pragma once
#include <Editor/Editor.h>
#include <Game/Game.h>

class GameEditor : public Editor
{
	RTTI_CLASS(GameEditor, Editor)
public:
	GameEditor();
	virtual void Update() override;

private:
	DC::Ref<Game> mGame;

	DC::String mWindowLabel;
};
