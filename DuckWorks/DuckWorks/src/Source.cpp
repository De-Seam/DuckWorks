#include "Precomp.h"
#include "Game/App/App.h"

#include "External/SDL/SDL.h"
#include "Game/App/FactoryRegistry.h"

int main(int, char**)
{
	gRegisterFactoryClasses();
	return gApp.Run();
}
