#pragma once

template<typename taType>
void Engine::CreateManager()
{
	int32 type_id = taType::sGetManagerTypeID();
	if (type_id <= mManagers.size())
		mManagers.resize(type_id + 1);
	RegisterManager(taType::sNewInstance());
}

template<typename taType>
taType& Engine::GetManager()
{
	return *reinterpret_cast<taType*>(mManagers[taType::sGetManagerTypeID()].Get());
}

template<typename taType>
taType* Engine::FindManager()
{
	int32 type_id = taType::sGetManagerTypeID();
	if (type_id < mManagers.size())
		return nullptr;
	return reinterpret_cast<taType*>(mManagers[taType::sGetManagerTypeID()].Get());
}