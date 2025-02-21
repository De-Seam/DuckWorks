#pragma once
#include <App/App.h>

class ChessApp : public App
{
public:
	ChessApp();

	virtual void Update(float inDeltaTime) override;
};