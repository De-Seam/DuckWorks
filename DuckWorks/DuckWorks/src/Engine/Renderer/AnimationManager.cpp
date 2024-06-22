#include "Precomp.h"
#include "Engine/Renderer/AnimationManager.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/World/World.h"

AnimationManager gAnimationManager;

void AnimationManager::Update(float inDeltaTime)
{
	for (auto it = mAnimations.begin(); it != mAnimations.end();)
	{
		if (auto animation = it->lock())
		{
			animation->Update(inDeltaTime);
			++it;
		}
		else
		{
			it = mAnimations.erase(it);
		}
	}
}
