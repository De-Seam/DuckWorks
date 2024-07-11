#pragma once
//Other Utilities
#include "Types.h"

// Core includes
#include "Core/Math/Math.h"

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
using StringView = std::string_view;
using Json = nlohmann::ordered_json;

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

// Casts
#define SCast static_cast
#define RCast reinterpret_cast
#define CCast const_cast
#define SPCast std::static_pointer_cast
#define RPCast std::reinterpret_pointer_cast

#define THREADLOCAL __declspec(thread)

#define gAssertImpl(inCondition, inMessage, ...) assert((inCondition) && (inMessage))
#define gAssert(...) gAssertImpl(__VA_ARGS__, "Assertion failed: " #__VA_ARGS__)

#define gDebugIf(inCondition, inStatement) if(inCondition) {inStatement;}

#define JSON_SAVE(inJson, inVariable) inJson[(#inVariable)] = (inVariable)

// See if the json contains the variable, if so, load it
#define JSON_TRY_LOAD(inJson, inVariable) if((inJson).contains((#inVariable))) (inVariable) = (inJson)[#inVariable].get<decltype(inVariable)>()

#ifdef _DEBUG
#define IF_DEBUG(inStatement) inStatement
#define IF_NOT_DEBUG(inStatement)
#define JSON_LOAD(inJson, inVariable) JSON_TRY_LOAD(inJson, inVariable)
#else // _DEBUG
#define IF_DEBUG(inStatement)
#define IF_NOT_DEBUG(inStatement) inStatement
#define JSON_LOAD(inJson, inVariable) inVariable = (inJson)[(#inVariable)].get<decltype(inVariable)>()
#endif // _DEBUG

// Utility functions
bool gIsValidTextureExtension(const std::string& inFilePath);
bool gIsValidLuaExtension(const std::string& inFilePath);

// JSON Serialization helper functions
// vec2
inline void to_json(Json& outJson, const Vec2& inVariable)
{
	outJson = Json{inVariable.mX, inVariable.mY};
}

inline void from_json(const Json& inJson, Vec2& outVariable)
{
	inJson.at(0).get_to(outVariable.mX);
	inJson.at(1).get_to(outVariable.mY);
}

// vec3
inline void to_json(Json& outJson, const Vec3& inVariable)
{
	outJson = Json{inVariable.mX, inVariable.mY, inVariable.mZ};
}

inline void from_json(const Json& inJson, Vec3& outVariable)
{
	inJson.at(0).get_to(outVariable.mX);
	inJson.at(1).get_to(outVariable.mY);
	inJson.at(2).get_to(outVariable.mZ);
}

// vec4
inline void to_json(Json& outJson, const Vec4& inVariable)
{
	outJson = Json{inVariable.mX, inVariable.mY, inVariable.mZ, inVariable.mW};
}

inline void from_json(const Json& inJson, Vec4& outVariable)
{
	inJson.at(0).get_to(outVariable.mX);
	inJson.at(1).get_to(outVariable.mY);
	inJson.at(2).get_to(outVariable.mZ);
	inJson.at(3).get_to(outVariable.mW);
};

// ivec2
inline void to_json(Json& outJson, const IVec2& inVariable)
{
	outJson = Json{inVariable.mX, inVariable.mY};
}

inline void from_json(const Json& inJson, IVec2& outVariable)
{
	inJson.at(0).get_to(outVariable.mX);
	inJson.at(1).get_to(outVariable.mY);
}

// Transform2D
inline void to_json(Json& outJson, const Transform2D& inVariable)
{
	outJson = Json{{"mPosition", inVariable.mPosition}, {"mHalfSize", inVariable.mHalfSize}, {"mRotation", inVariable.mRotation}};
}

inline void from_json(const Json& inJson, Transform2D& outVariable)
{
	inJson.at("mPosition").get_to(outVariable.mPosition);
	inJson.at("mHalfSize").get_to(outVariable.mHalfSize);
	inJson.at("mRotation").get_to(outVariable.mRotation);
}

// Ref Object
// We only define to_json
// Because from_json needs to use a specific Factory and Construct Variables
class RTTIBaseClass;
void to_json(Json& outJson, RTTIBaseClass* inVariable);
void to_json(Json& outJson, const SharedPtr<RTTIBaseClass>& inVariable);
