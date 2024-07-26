#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>

// Engine includes
#include <Engine/Engine.h>

class App : public RTTIClass
{
	RTTI_CLASS(App, RTTIClass)

public:
	App();
	virtual ~App();

	void Run(); ///< Run the app

	void RequestShutdown();

private:
	void MainLoop();
	void Update(float inDeltaTime);

	bool mShutdownRequested = false;
};