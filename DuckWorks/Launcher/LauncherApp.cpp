#include <DuckWorks/Launcher/LauncherApp.h>

#include <Engine/Events/SDLEventManager.h>
#include <Engine/Renderer/Renderer.h>

#include <External/imgui/imgui.h>

LauncherApp::~LauncherApp() {}

void LauncherApp::Update(float inDeltaTime)
{
	ImGui::Begin("Launcher");

	sGetAppConstructors().ForEach([](const DC::String& inName, const std::function<DC::UniquePtr<App>(void)>& inConstructFunction)
	{
		// 3 = length of "App"
		DC::String name = inName.SubStr(0, inName.Length() - 3);
		if (ImGui::Button(name.CStr()))
			sSetActiveApp(Move(inConstructFunction()));
	});

	ImGui::End();
}
