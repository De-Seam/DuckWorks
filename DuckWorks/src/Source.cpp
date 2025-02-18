#include "Precomp.h"

#include <DuckCore/Threads/Thread.h>

#include <Game/App/App.h>

#include "External/SDL/SDL.h"
#include "External/sol/sol.hpp"

using namespace DC;

int main(int, char**)
{
	gVerify(gIsMainThread());
	return gApp.Run();
}
