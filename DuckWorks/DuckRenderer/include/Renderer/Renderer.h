#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>
#include <Core/Containers/Array.h>

namespace sf
{
	class Drawable;
}

class Renderer : public RTTIClass
{
	RTTI_CLASS(Renderer, RTTIClass)

public:
	Renderer();
	~Renderer();

	void Draw(sf::Drawable& inDrawable);
	void Draw(Array<sf::Drawable&>& inDrawables);
};
