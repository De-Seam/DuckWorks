#pragma once
//Other Utilities
#include "Types.h"

// External libraries
#include <External/nlohmann/json.hpp>
#include <External/entt/entt.hpp>
#include <External/phmap/phmap.h>

// STD includes
#include <string>
#include <vector>
#include <memory>

using String = std::string;

inline void gAssert(bool inCondition, const String& inMessage = "")
{
	assert(inCondition && inMessage.c_str());
}
