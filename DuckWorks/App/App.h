#pragma once

class Engine;

// App base class
class App
{
public:
	App();
	virtual ~App();
	virtual void Update(float inDeltaTime) = 0;

	virtual bool ShouldShutdown() const;
};