#pragma once
#include <App/App.h>

class ChessApp : public App
{
public:
	static void sRegisterRTTI();

	ChessApp();

	virtual void Update(float inDeltaTime) override;
};