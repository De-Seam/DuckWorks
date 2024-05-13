#pragma once
//Other Utilities
#include "Types.h"

// Core includes
#include "Core/Math/FMath.h"

// External libraries
#include <External/nlohmann/json.hpp>
#include <External/phmap/phmap.h>

// STD includes
#include <atomic>
#include <memory>
#include <string>
#include <vector>

#define PROFILE_MUTEXES 1

using String = std::string;
using Json = nlohmann::json;

// Smart Pointers
template<typename taType>
using UniquePtr = std::unique_ptr<taType>;

template<typename taType>
using SharedPtr = std::shared_ptr<taType>;

template<typename taType>
using WeakPtr = std::weak_ptr<taType>;

#define gMakeUnique std::make_unique
#define gMakeShared std::make_shared

// Containers
_EXPORT_STD inline constexpr std::nullopt_t NullOpt{std::nullopt_t::_Tag{}};

template<typename taType>
using Optional = std::optional<taType>;

template<typename taType, class taAllocator = std::allocator<taType>>
using Array = std::vector<taType, taAllocator>;

template<typename taType, size_t size>
using StaticArray = std::array<taType, size>;

template<typename taKey, typename taValue>
using HashMap = phmap::flat_hash_map<taKey, taValue>;

template<typename taTypeA, typename taTypeB>
using Pair = std::pair<taTypeA, taTypeB>;

template<typename taType>
using Atomic = std::atomic<taType>;

template<typename taType>
using Function = std::function<taType>;

class Entity;
using EntityPtr = SharedPtr<Entity>;
using EntityWeakPtr = WeakPtr<Entity>;

// Casts
#define SCast static_cast
#define RCast reinterpret_cast
#define CCast const_cast
#define SPCast std::static_pointer_cast
#define RPCast std::reinterpret_pointer_cast

#define THREADLOCAL __declspec(thread)

#define gAssert(inCondition, inMessage) assert((inCondition) && (inMessage))

#define gDebugIf(inCondition, inStatement) if(inCondition) {inStatement;}

#define JSON_SAVE(inJson, inVariable) inJson[(#inVariable)] = (inVariable)

// See if the json contains the variable, if so, load it
#define JSON_TRY_LOAD(inJson, inVariable) if((inJson).contains((#inVariable))) (inVariable) = (inJson)[#inVariable].get<decltype(inVariable)>()

#define JSON_LOAD(inJson, inVariable) inVariable = (inJson)[(#inVariable)].get<decltype(inVariable)>()

// Utility functions
bool gIsValidTextureExtension(const std::string& inFilePath);

// JSON Serialization helper functions
namespace fm
{
// vec2
inline void to_json(Json& outJson, const vec2& inVariable)
{
	outJson = Json{inVariable.x, inVariable.y};
}

inline void from_json(const Json& inJson, vec2& outVariable)
{
	inJson.at(0).get_to(outVariable.x);
	inJson.at(1).get_to(outVariable.y);
}

// vec3
inline void to_json(Json& outJson, const vec3& inVariable)
{
	outJson = Json{inVariable.x, inVariable.y, inVariable.z};
}

inline void from_json(const Json& inJson, vec3& outVariable)
{
	inJson.at(0).get_to(outVariable.x);
	inJson.at(1).get_to(outVariable.y);
	inJson.at(2).get_to(outVariable.z);
}

// vec4
inline void to_json(Json& outJson, const vec4& inVariable)
{
	outJson = Json{inVariable.x, inVariable.y, inVariable.z, inVariable.w};
}

inline void from_json(const Json& inJson, vec4& outVariable)
{
	inJson.at(0).get_to(outVariable.x);
	inJson.at(1).get_to(outVariable.y);
	inJson.at(2).get_to(outVariable.z);
	inJson.at(3).get_to(outVariable.w);
};

// ivec2
inline void to_json(Json& outJson, const ivec2& inVariable)
{
	outJson = Json{inVariable.x, inVariable.y};
}

inline void from_json(const Json& inJson, ivec2& outVariable)
{
	inJson.at(0).get_to(outVariable.x);
	inJson.at(1).get_to(outVariable.y);
}

// Transform2D
inline void to_json(Json& outJson, const Transform2D& inVariable)
{
	outJson = Json{{"Position", inVariable.position}, {"HalfSize", inVariable.halfSize}, {"Rotation", inVariable.rotation}};
}

inline void from_json(const Json& inJson, Transform2D& outVariable)
{
	inJson.at("Position").get_to(outVariable.position);
	inJson.at("HalfSize").get_to(outVariable.halfSize);
	inJson.at("Rotation").get_to(outVariable.rotation);
}
} // Namespace fm

// TextureResource
class TextureResource;
void to_json(Json& outJson, const SharedPtr<TextureResource>& inVariable);
void from_json(const Json& inJson, SharedPtr<TextureResource>& outVariable);

// AABB
struct AABB;
void to_json(Json& outJson, const AABB& inVariable);
void from_json(const Json& inJson, AABB& outVariable);
