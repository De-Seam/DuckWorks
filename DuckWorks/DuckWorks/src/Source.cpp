#include "Precomp.h"
#include "Game/App/App.h"

#define SDL_MAIN_HANDLED
#include "External/SDL/SDL.h"
#include "External/sol/sol.hpp"

int main(int, char**)
{
	return gApp.Run();
}
