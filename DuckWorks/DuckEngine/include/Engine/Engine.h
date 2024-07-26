#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Containers/HashMap.h>
#include <Core/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>
#include <Core/Threads/Thread.h>
#include <Core/Containers/SmartPointers.h>

// Renderer includes
#include <Renderer/Renderer.h>

class Renderer;

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

	Renderer mRenderer;
};

THREADLOCAL extern UniquePtr<Engine> gEngine;

#include "Engine.inl"
