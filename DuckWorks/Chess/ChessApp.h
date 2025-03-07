#pragma once
#include <DuckWorks/App/App.h>

class ChessApp : public App
{
public:
	ChessApp();

	virtual void Update(float inDeltaTime) override;
};