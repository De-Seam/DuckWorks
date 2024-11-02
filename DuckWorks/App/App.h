#pragma once
// DuckCore includes
#include <DuckCore/RTTI/Ref.h>

class Engine;

// App base class
class App
{
public:
	App();
	virtual ~App();
	virtual void Update(float inDeltaTime);

	virtual bool ShouldShutdown() const;

private:
	DC::Ref<Engine> mEngine;
};