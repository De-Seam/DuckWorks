#pragma once
#include "Core/CoreBase.h"

// Managers are classes that are created as extern global variables
class Manager : public RTTIBaseClass
{
	RTTI_VIRTUAL_CLASS(Manager, RTTIBaseClass)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	Manager(const ConstructParameters& inParameters) : Base(inParameters) {}

	virtual void Init() {}
	virtual void Shutdown() {}
	virtual void Update([[maybe_unused]] float inDeltaTime) {}

	bool RequiresUpdate() const { return mRequiresUpdate; }
	bool ShouldUpdateWhilePaused() const { return mUpdateWhilePaused; }

protected:
	bool mRequiresUpdate = false; ///< Set in Constructor if the manager needs an update
	bool mUpdateWhilePaused = true; ///< Set in Constructor if the manager should update while the game is paused
};

#define REGISTER_MANAGER(inManager) \
	gEngine.RegisterManager(inManager)

#define INIT_MANAGER_BEFORE(inManager, inOtherManager) \
	gEngine.InitManagerBefore(inManager, inOtherManager)

#define INIT_MANAGER_AFTER(inManager, inOtherManager) \
	gEngine.InitManagerAfter(inManager, inOtherManager)

#define UPDATE_MANAGER_BEFORE(inManager, inOtherManager) \
	gEngine.UpdateManagerBefore(inManager, inOtherManager)

#define UPDATE_MANAGER_AFTER(inManager, inOtherManager) \
	gEngine.UpdateManagerAfter(inManager, inOtherManager)
