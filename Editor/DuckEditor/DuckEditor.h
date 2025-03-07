#pragma once
#include <Editor/Editor.h>

class DuckEditor : public Editor
{
	RTTI_CLASS(DuckEditor, Editor)
public:
	virtual void Update(float aDeltaTime) override;

private:

};