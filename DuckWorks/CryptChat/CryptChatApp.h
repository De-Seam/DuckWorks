#pragma once
#include <DuckWorks/App/App.h>

class CryptChatApp : public App
{
public:
	CryptChatApp();
	virtual ~CryptChatApp() override;

	virtual void Update(float inDeltaTime) override;
};