#include <DuckWorks/App/App.h>

#include <DuckCore/Core/Log.h>

#include <Engine/Engine.h>
#include <DuckWorks/Chess/ChessApp.h>
#include <DuckWorks/CryptChat/CryptChatApp.h>
#include <DuckWorks/Editor/EditorApp.h>
#include <DuckWorks/Game/GameApp.h>
#include <DuckWorks/Launcher/LauncherApp.h>
#include <DuckWorks/Sandbox/SandboxApp.h>

#define REGISTER_APP(inApp) App::sRegisterAppConstructor(#inApp, []() { return DC::Move(DC::MakeUnique<inApp>()); })

using namespace DC;

void App::sStartup()
{
	REGISTER_APP(LauncherApp);
	REGISTER_APP(GameApp);
	REGISTER_APP(EditorApp);
	REGISTER_APP(SandboxApp);
	REGISTER_APP(CryptChatApp);
	REGISTER_APP(ChessApp);
}

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
		Log(ELogLevel::Warning, String::sFormatted("App name %s not found", *aAppName));
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
