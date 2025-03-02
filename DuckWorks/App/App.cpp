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

	HashMap<String, std::function<UniquePtr<App>(void)>>::Iterator iter = sAppConstructors.Find(app_name);
	if (!iter.IsValid())
	{
		Log(ELogLevel::Warning, String::sFormatted("App name %s not found", aAppName));
		gAssert(false);
		return;
	}
	const std::function<UniquePtr<App>(void)>& construct_function = iter.GetValue();
	sSetActiveApp((construct_function)());
}

void App::sRegisterAppConstructor(const String& aName, std::function<UniquePtr<App>()> aConstructFunction)
{
	gAssert(!sAppConstructors.Contains(aName));
	sAppConstructors[aName] = Move(aConstructFunction);
}
