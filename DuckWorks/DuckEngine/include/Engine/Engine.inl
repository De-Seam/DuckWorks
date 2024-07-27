#pragma once

template<typename taType>
void Engine::CreateManager()
{
	static_assert(std::is_base_of<Manager, taType>());
	RegisterManager(taType::sNewInstance());
}

template<typename taType>
taType& Engine::GetManager()
{
	static_assert(std::is_base_of<Manager, taType>());
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