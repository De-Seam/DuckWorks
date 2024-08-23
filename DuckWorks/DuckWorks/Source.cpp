#include <iostream>

#include <Core/Module/Module.h>
#include <Core/Module/ModuleManager.h>

class DuckWorksModule : public Module
{
	RTTI_CLASS(DuckWorksModule, Module)
public:
	virtual void Init() {};
	virtual void Deinit() {};
};

int main()
{
	if (gModuleManager == nullptr)
	{
		// Core Module was not loaded correctly
		gAssert(false);
		return 0;
	}
	gModuleManager->CreateModule<DuckWorksModule>();
	return 0;
}