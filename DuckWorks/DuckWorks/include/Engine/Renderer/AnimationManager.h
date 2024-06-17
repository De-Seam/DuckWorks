#pragma once
#include "AnimationBase.h"

class World;

class AnimationManager
{
public:
	// Create a new animation for an entity. Ownership is controlled through the shared_ptr. If it gets destroyed then it will be gone.
	template<typename taType, typename... taArgs>
	[[nodiscard]] std::shared_ptr<taType> CreateAnimation(taArgs&&... inArgs);

	void Update(float inDeltaTime);

private:
	std::vector<std::weak_ptr<AnimationBase>> mAnimations;
};

extern AnimationManager gAnimationManager;

template<typename taType, typename... taArgs>
std::shared_ptr<taType> AnimationManager::CreateAnimation(taArgs&&... inArgs)
{
	taType type = taType(std::forward<taArgs>(inArgs)...);
	std::shared_ptr<taType> animation = std::make_shared<taType>(taType(std::forward<taArgs>(inArgs)...));
	mAnimations.push_back(animation);
	return animation;
}
