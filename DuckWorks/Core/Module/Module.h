#pragma once
// Core includes
#include <Core/Config.h>
#include <Core/RTTI/RTTI.h>

class CORE_API Module : public RTTIClass
{
	RTTI_VIRTUAL_CLASS(Module, RTTIClass)
public:
	virtual void Init() = 0;
	virtual void Deinit() = 0;

private:
};