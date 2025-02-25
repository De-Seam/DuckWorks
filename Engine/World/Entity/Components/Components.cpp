#include <Engine/World/Entity/Components/Components.h>

#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Core/Log.h>
#include <DuckCore/RTTI/RTTI.h>

using namespace DC;

struct ComponentFunctions
{
	std::function<Component*(ECSEntity&)> mAddComponent;
	std::function<void(ECSEntity&)> mRemoveComponent;
	std::function<Component*(ECSEntity&)> mFindComponentFunction;
	std::function<const Component*(const ECSEntity&)> mFindConstComponentFunction;
	std::function<bool(const ECSEntity&)> mHasComponent;
	std::function<void(ECSEntity&, const Json&)> mFromJson;
	std::function<Json(const ECSEntity&)> mToJson;
};

HashMap<const RTTI*, ComponentFunctions> gRTTIToComponentFunctions;


const RTTI* FindComponentRTTIByName(const String& aName)
{
	for (auto& [key, value] : gRTTIToComponentFunctions)
	{
		if (aName == key->GetClassName())
			return key;
	}
	return nullptr;
}

ComponentFunctions* FindComponentFunctionsOrError(const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = gRTTIToComponentFunctions.Find(&aComponentRTTI);
	if (component_functions == nullptr)
	{
		Log<LogCategoryComponents>(ELogLevel::Error, String::sFormatted("Component %s not found", aComponentRTTI.GetClassName()));
		gAssert(false);
		return nullptr;
	}
	return component_functions;
}

Component* AddComponent(ECSEntity& aECSEntity, const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return nullptr;

	return component_functions->mAddComponent(aECSEntity);
}

void RemoveComponent(ECSEntity& aECSEntity, const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return;

	component_functions->mRemoveComponent(aECSEntity);
}

Component* FindComponent(ECSEntity& aECSEntity, const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return nullptr;

	return component_functions->mFindComponentFunction(aECSEntity);
}

const Component* FindComponent(const ECSEntity& aECSEntity, const DC::RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return nullptr;

	return component_functions->mFindConstComponentFunction(aECSEntity);
}

bool HasComponent(const ECSEntity& aECSEntity, const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return false;

	return component_functions->mHasComponent(aECSEntity);
}

void FromJson(ECSEntity& aECSEntity, const RTTI& aComponentRTTI, const Json& aJson)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return;

	component_functions->mFromJson(aECSEntity, aJson);
}

Json ToJson(const ECSEntity& aECSEntity, const RTTI& aComponentRTTI)
{
	ComponentFunctions* component_functions = FindComponentFunctionsOrError(aComponentRTTI);
	if (component_functions == nullptr)
		return {};

	return component_functions->mToJson(aECSEntity);
}

void GetAllComponentRTTIs(Array<const RTTI*> outComponentRTTIs)
{
	gAssert(outComponentRTTIs.IsEmpty());
	for (auto& [key, value] : gRTTIToComponentFunctions)
		outComponentRTTIs.Add(key);
}

void Internal::RegisterComponentFunctions(
	const RTTI& aComponentRTTI,
	std::function<Component*(ECSEntity&)> aAddComponentFunction,
	std::function<void(ECSEntity&)> aRemoveComponentFunction,
	std::function<Component*(ECSEntity&)> aFindComponentFunction,
	std::function<const Component*(const ECSEntity&)> aFindConstComponentFunction,
	std::function<bool(const ECSEntity&)> aHasComponentFunction, 
	std::function<void(ECSEntity&, const Json&)> aFromJsonFunction, 
	std::function<Json(const ECSEntity&)> aToJsonFunction)
{
	gAssert(!gRTTIToComponentFunctions.Contains(&aComponentRTTI));

	gRTTIToComponentFunctions[&aComponentRTTI] =
		ComponentFunctions
		(
			Move(aAddComponentFunction),
			Move(aRemoveComponentFunction),
			Move(aFindComponentFunction),
			Move(aFindConstComponentFunction),
			Move(aHasComponentFunction),
			Move(aFromJsonFunction),
			Move(aToJsonFunction)
		);
}
