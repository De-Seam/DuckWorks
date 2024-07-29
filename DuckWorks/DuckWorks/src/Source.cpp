#include "Precomp.h"

// Core includes
#include <Core/Utilities/Assert.h>

// Renderer includes
#include <Engine/Renderer/Renderer.h>

// App includes
#include <App/App.h>
#include <App/AppModule.h>

#include <External/SFML/Graphics.hpp>

int main(int, char**)
{
	AppModule app_module;
	app_module.RegisterRTTI();

	App app;
	app.Run();

	return 0;
}

#ifdef _WIN32

namespace Win
{
#include <Windows.h>
}
int APIENTRY WinMain(
	Win::HINSTANCE,
	Win::HINSTANCE,
	Win::LPSTR, int)
{
	return main(__argc, __argv);
}
#endif
