#pragma once

// External includes
#include <External/phmap/phmap.h>

template<typename taKeyType, typename taValueType>
using HashMap = phmap::flat_hash_map<taKeyType, taValueType>;
