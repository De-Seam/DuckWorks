#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>

// App base class
class App
{
public:
	App();
	virtual ~App();

	virtual void Update(float inDeltaTime) = 0;

	static void sSetActiveApp(const DC::String& aAppName);
	static void sSetActiveApp(DC::UniquePtr<App> inApp) { sActiveApp = Move(inApp); }
	static void sClearActiveApp() { sActiveApp.Delete(); }
	static App* sGetActiveApp() { return sActiveApp; }
	static void sRegisterAppConstructor(const DC::String& aName, std::function<DC::UniquePtr<App>(void)> aConstructFunction);
	static const DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>>& sGetAppConstructors() { return sAppConstructors; }

private:
	inline static DC::UniquePtr<App> sActiveApp;
	inline static DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>> sAppConstructors; // Maps App name to constructor function
};
