#pragma once
#include <Engine/Manager/Manager.h>

class DebugManager : public Manager
{
	RTTI_MANAGER(DebugManager, Manager)
public:
	DebugManager();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float inDeltaTime) override;

private:
};