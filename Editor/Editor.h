#pragma once
#include <Engine/Objects/Object.h>

class Editor : public Object
{
	RTTI_CLASS(Editor, Object)
public:
	Editor();
	explicit Editor(const DC::Json& aJson);
	virtual ~Editor() override;
	virtual DC::Json ToJson() const override;

private:
	
};