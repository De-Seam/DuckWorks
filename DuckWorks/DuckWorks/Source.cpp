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
	gModuleManager = new ModuleManager;
	gModuleManager->CreateModule<DuckWorksModule>();
	return 0;
}