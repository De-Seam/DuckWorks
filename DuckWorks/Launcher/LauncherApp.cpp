#include <Engine/Events/SDLEventManager.h>
#include <Engine/Renderer/Renderer.h>

#include <External/imgui/imgui.h>

#include <Launcher/LauncherApp.h>

LauncherApp::LauncherApp()
{
	gEngine->TryCreateManager<Renderer>();
}

LauncherApp::~LauncherApp() {}

void LauncherApp::Update(float inDeltaTime)
{
	ImGui::Begin("Launcher");

	sGetAppConstructors().ForEach([](const DC::String& inName, const std::function<DC::UniquePtr<App>(void)>& inConstructFunction)
	{
		if (ImGui::Button(inName.CStr()))
			sSetActiveApp(gMove(inConstructFunction()));
	});

	ImGui::End();
}
