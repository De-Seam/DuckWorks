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

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float inDeltaTime) = 0;
};
