#pragma once
// Std includes
#include <vector>

template<typename taType>
using Array = std::vector<taType>;

template<typename taType, size_t taSize>
using StaticArray = std::array<taType, taSize>;