#pragma once
// Core includes
#include <Core/Config.h>
#include <Core/Containers/HashMap.h>
#include <Core/Module/Module.h>

class CORE_API ModuleManager
{
public:
	template<typename taType>
	taType* CreateModule();

	template<typename taType>
	void DestroyModule();

	template<typename taType>
	taType* GetModule();

	template<typename taType>
	taType* FindModule();

private:
	HashMap<RTTITypeID, Module*> mModules;
};

extern ModuleManager* gModuleManager;

template<typename taType>
inline taType* ModuleManager::CreateModule()
{
	gAssert(!mModules.contains(taType::sGetRTTI().GetTypeID()));

	taType* module = new taType;
	mModules[module->GetRTTI()->GetID()] = module;
	module->Init();
	return module;
}

template<typename taType>
inline void ModuleManager::DestroyModule()
{
	gAssert(mModules.contains(taType::sGetRTTI().GetTypeID()));

	Module* module = mModules[taType::sGetRTTI().GetTypeID()];
	module->Deinit();
	delete module;
	mModules.erase(taType::sGetRTTI().GetTypeID());
}

template<typename taType>
inline taType* ModuleManager::GetModule()
{
	gAssert(mModules.contains(taType::sGetRTTI().GetTypeID()));

	return static_cast<taType*>(mModules[taType::sGetRTTI().GetTypeID()]);
}

template<typename taType>
inline taType* ModuleManager::FindModule()
{
	auto it = mModules.find(taType::sGetRTTI().GetTypeID());
	if (it.is_valid())
		return *it;
	return nullptr;
}
