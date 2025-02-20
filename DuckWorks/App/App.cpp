#include <App/App.h>

#include <Engine/Engine.h>

App::App()
{
	gAssert(gEngine != nullptr);
}

App::~App()
{
	gAssert(gEngine != nullptr);
}

void App::sRegisterAppConstructor(const DC::String& aName, std::function<DC::UniquePtr<App>()> aConstructFunction)
{
	gAssert(!sAppConstructors.Contains(aName));
	sAppConstructors[aName] = Move(aConstructFunction);
}
