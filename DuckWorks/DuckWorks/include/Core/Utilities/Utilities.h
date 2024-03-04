#pragma once
//Other Utilities
#include "Types.h"

// External libraries
#include <External/nlohmann/json.hpp>
#include <External/phmap/phmap.h>

// STD includes
#include <string>
#include <vector>
#include <memory>

using String = std::string;
using Json = nlohmann::json;

// Smart Pointers
template<typename taType>
using UniquePtr = std::unique_ptr<taType>;

template<typename taType>
using SharedPtr = std::shared_ptr<taType>;

template<typename taType>
using WeakPtr = std::weak_ptr<taType>;

// Containers
template<typename taType>
using Array = std::vector<taType>;

template<typename taKey, typename taValue>
using HashMap = phmap::flat_hash_map<taKey, taValue>;

// Casts
#define SCast static_cast
#define RCast reinterpret_cast
#define CCast const_cast
#define DCast dynamic_cast
#define SPCast std::static_pointer_cast
#define DPCast std::dynamic_pointer_cast
#define RPCast std::reinterpret_pointer_cast

#define Cast SCast

#define gAssert(inCondition, inMessage) assert((inCondition) && (inMessage))
