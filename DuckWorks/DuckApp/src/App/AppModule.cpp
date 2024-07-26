#include <App/Precomp.h>
#include <App/AppModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// App includes
#include <App/App.h>

THREADLOCAL AppModule* gAppModule = nullptr;

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
	mRTTIFactory.RegisterClass<App>();
}
