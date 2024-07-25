#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Containers/HashMap.h>
#include <Core/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>

// Std include
#include <memory>

class Engine : public RTTIClass
{
	RTTI_CLASS(Engine, RTTIClass)

public:
	void Init();
	void Shutdown();
	void Update(float inDeltaTime);

	template<typename taType>
	void CreateManager();
	void RegisterManager(Manager* inManager); ///< Register a manager without owning it
	template<typename taType>
	taType& GetManager();
	template<typename taType>
	taType* FindManager();

private:
	void RegisterManagers();

	Array<Ref<Manager>> mManagers;
	Array<Manager*> mManagersToUpdate;
};

extern Engine gEngine;

#include "Engine.inl"
