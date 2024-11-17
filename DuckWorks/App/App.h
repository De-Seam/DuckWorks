#pragma once

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

	virtual bool ShouldShutdown() const;
};