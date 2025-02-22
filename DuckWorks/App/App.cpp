#include <App/App.h>

#include <DuckCore/Core/Log.h>

#include <Engine/Engine.h>

using namespace DC;

App::App()
{
	gAssert(gEngine != nullptr);
}

App::~App()
{
	gAssert(gEngine != nullptr);
}

void App::sSetActiveApp(const String& aAppName)
{
	String app_name = aAppName;
	if (!app_name.Contains("App"))
		app_name += "App";

	std::function<DC::UniquePtr<App>(void)>* construct_function = sAppConstructors.Find(app_name);
	if (construct_function == nullptr)
	{
		Log(ELogLevel::Warning, String::sFormatted("App name %s not found", aAppName));
		gAssert(false);
		return;
	}
	sSetActiveApp((*construct_function)());
}

void App::sRegisterAppConstructor(const String& aName, std::function<UniquePtr<App>()> aConstructFunction)
{
	gAssert(!sAppConstructors.Contains(aName));
	sAppConstructors[aName] = Move(aConstructFunction);
}
