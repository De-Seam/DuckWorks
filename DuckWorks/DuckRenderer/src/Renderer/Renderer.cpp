#include "Renderer/Precomp.h"
#include <Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

Renderer::Renderer()
{
	sf::VideoMode video_mode;
	video_mode.width = 1280;
	video_mode.height = 720;
	sf::RenderWindow window(video_mode, "DuckRenderer");
}

Renderer::~Renderer()
{
}

void Renderer::Draw(sf::Drawable& inDrawable)
{

}

void Renderer::Draw(Array<sf::Drawable&>& inDrawables)
{
}
