#include <Precomp.h>
#include <App/AppModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// App includes
#include <App/App.h>

AppModule* gAppModule = nullptr;

AppModule::AppModule()
{
	gAssert(gAppModule == nullptr);
	gAppModule = this;
}

AppModule::~AppModule()
{
	gAssert(gAppModule != nullptr);
	gAppModule = nullptr;
}

void AppModule::RegisterRTTI()
{
	PROFILE_SCOPE(AppModule::RegisterRTTI)

	EngineModule::RegisterRTTI();

	mRTTIFactory.RegisterClass<App>();
}
