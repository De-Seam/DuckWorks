#pragma once
#include <memory>

template<typename taType>
using UniquePtr = std::unique_ptr<taType>;

template<typename taType>
using SharedPtr = std::shared_ptr<taType>;

template<typename taType>
using WeakPtr = std::weak_ptr<taType>;

template<typename taType, typename... taArgs>
inline SharedPtr<taType> gMakeShared(taArgs&&... inArgs)
{
	return std::make_shared<taType>(std::forward(inArgs)...);
}

template<typename taType, typename... taArgs>
inline UniquePtr<taType> gMakeUnique(taArgs&&... inArgs)
{
	return std::make_unique<taType>(std::forward(inArgs)...);
}