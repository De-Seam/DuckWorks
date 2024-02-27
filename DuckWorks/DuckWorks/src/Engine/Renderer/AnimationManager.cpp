#include "Precomp.h"
#include "Engine/Renderer/AnimationManager.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/World/World.h"

AnimationManager gAnimationManager;

void AnimationManager::Update(World* inWorld, float inDeltaTime)
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

	auto animation_view = inWorld->GetRegistry().view<AnimationComponent, TextureRenderComponent>();
	animation_view.each([=](AnimationComponent& inAnimationComponent, TextureRenderComponent& inRenderComponent)
	{
		inRenderComponent.mUseSrcRect = true;
		inAnimationComponent.mTimeSinceUpdate += inDeltaTime;
		AnimationBase::Frame& current_frame = inAnimationComponent.mCurrentFrame;
		inRenderComponent.mFlip = inAnimationComponent.mAnimation->GetFlip();
		gAssert(current_frame.mDuration > 0.f, "Duration should be higher then 0!");
		while (inAnimationComponent.mTimeSinceUpdate >= current_frame.mDuration)
		{
			inAnimationComponent.mTimeSinceUpdate -= current_frame.mDuration;
			current_frame = inAnimationComponent.mAnimation->IncrementFrame();
			inRenderComponent.mSrcRect = {current_frame.mPosition, current_frame.mSize};
		}
	});
}
