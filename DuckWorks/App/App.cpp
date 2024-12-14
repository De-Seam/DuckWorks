#include <App/App.h>

// Core includes
#include <DuckCore/Containers/SharedPtr.h>

// Engine includes
#include <Engine/Engine.h>

App::App()
{
	gAssert(gEngine != nullptr);
}

App::~App()
{
	gAssert(gEngine != nullptr);
}

void App::sRegisterAppConstructor(const DC::String& inName, std::function<DC::UniquePtr<App>()> inConstructFunction)
{
	gAssert(!sAppConstructors.Contains(inName));
	sAppConstructors[inName] = gMove(inConstructFunction);
}
