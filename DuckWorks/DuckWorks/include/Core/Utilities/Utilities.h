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

// Smart Pointers
#define UniquePtr std::unique_ptr
#define SharedPtr std::shared_ptr
#define WeakPtr std::weak_ptr

// Casts
#define SCast static_cast
#define RCast reinterpret_cast
#define CCast const_cast
#define DCast dynamic_cast
#define SPCast std::static_pointer_cast
#define DPCast std::dynamic_pointer_cast
#define RPCast std::reinterpret_pointer_cast

#define Cast SCast

inline void gAssert(bool inCondition, const String& inMessage = "")
{
	assert(inCondition && inMessage.c_str());
}
