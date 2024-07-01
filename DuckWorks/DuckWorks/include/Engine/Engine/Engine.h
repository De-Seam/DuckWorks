#pragma once
#include "Core/CoreBase.h"

#define SOL_ALL_SAFETIES_ON 1
#include "External/sol/sol.hpp"

class Engine
{
public:
	void Init();
	void Shutdown();

	bool FileExists(const char* inFilePath) const;

	sol::state& GetLua() { return mLua; }

private:
	sol::state mLua;
};

extern Engine gEngine;
