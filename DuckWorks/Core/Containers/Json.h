#pragma once
// External includes
#include <Core/External/nlohmann/json.hpp>

// Std includes
#include <type_traits>

using Json = nlohmann::ordered_json;


template <typename, typename T>
struct has_serialize {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

template <typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
    template <typename T>
    static constexpr auto check(T*) ->
        typename std::is_same<decltype(std::declval<T>().Serialize()), Ret>::type;

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename, typename T>
struct has_deserialize {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

template <typename C, typename Ret, typename... Args>
struct has_deserialize<C, Ret(Args...)> {
private:
    template <typename T>
    static constexpr auto check(T*) ->
        typename std::is_same<decltype(std::declval<T>().Deserialize(std::declval<Args>()...)), Ret>::type;

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

#define JSON_SAVE(ioJson, inValue) (ioJson)[#inValue] = inValue
#define JSON_LOAD(inJson, outValue) if (inJson.contains(#outValue)) outValue = (inJson)[#outValue]