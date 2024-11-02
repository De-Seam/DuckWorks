#pragma once
// DuckCore includes
#include <DuckCore/RTTI/RTTIRefClass.h>

class Engine;

extern Engine* gEngine;

class Engine : public DC::RTTIRefClass
{
public:
	Engine();
	virtual ~Engine() override;

	void Update(float inDeltaTime);

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

private:
	bool mShouldShutdown = false;
};