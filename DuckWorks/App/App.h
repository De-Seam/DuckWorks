#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/RTTI/Ref.h>

#include <Engine/Engine.h>
#include <Engine/Manager.h>

class Engine;

// App base class
class App
{
public:
	App();
	virtual ~App();

	virtual void Update(float inDeltaTime) = 0;

	static void sSetActiveApp(DC::UniquePtr<App> inApp) { sActiveApp = gMove(inApp); }
	static void sClearActiveApp() { sActiveApp.Delete(); }
	static App* sGetActiveApp() { return sActiveApp; }
	static void sRegisterAppConstructor(const DC::String& inName, std::function<DC::UniquePtr<App>(void)> inConstructFunction);
	static const DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>>& sGetAppConstructors() { return sAppConstructors; }

private:
	inline static DC::UniquePtr<App> sActiveApp;
	inline static DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>> sAppConstructors; // Maps App name to constructor function
};
