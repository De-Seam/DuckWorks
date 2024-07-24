#pragma once

template<typename taType>
void Engine::CreateManager()
{
	int32 type_id = taType::sGetManagerTypeID();
	if (type_id <= mManagers.size())
		mManagers.resize(type_id + 1);
	mManagers[type_id] = std::make_unique<taType>();
}

template<typename taType>
taType& Engine::GetManager()
{
	return *mManagers[taType::sGetRTTI()->GetTypeID()];
}

template<typename taType>
taType* Engine::FindManager()
{
	int32 type_id = taType::sGetManagerTypeID();
	if (type_id < mManagers.size())
		return nullptr;
	return mManagers[type_id];
}