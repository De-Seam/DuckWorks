#include "Precomp.h"

#include <DuckCore/Threads/Thread.h>

#include <Game/App/App.h>

#include "External/SDL/SDL.h"
#include "External/sol/sol.hpp"

int main(int, char**)
{
	gVerify(DC::gIsMainThread());
	return gApp.Run();
}
