#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "External/SDL/SDL_render.h"

// AnimationBase class. The uint16 state is an enum
class AnimationBase
{
public:
	struct Frame
	{
		fm::ivec2 mPosition = {};
		fm::ivec2 mSize = {};
		float mDuration = 0.1f; ///< Time until next frame
	};

	// Update controls the state, but should not set the animation frame.
	// This should only read variables outside its class since it will be multi-threaded.
	virtual void Update(float inDeltaTime) = 0;

	void AddAnimation(uint16 inState, const std::vector<Frame>& inFrames)
	{
		mAnimations[inState] = inFrames;
	}

	void AddFrame(uint16 inState, const Frame& inFrame)
	{
		mAnimations[inState].push_back(inFrame);
	}

	void SetState(uint16 inState, int32 inFrame = 0)
	{
		if (mCurrentState == inState)
			return;

		gAssert(mAnimations.find(inState) != mAnimations.end(), "Animation state was not set!");
		gAssert(inFrame < mAnimations[inState].size(), "inFrame was invalid!");
		mCurrentState = inState;
		mCurrentAnimationIndex = inFrame;
		mCurrentFrame = mAnimations[inState][inFrame];
	}

	const Frame& IncrementFrame()
	{
		std::vector<Frame>& frames = mAnimations[mCurrentState];
		mCurrentAnimationIndex = (mCurrentAnimationIndex + 1) % frames.size();
		mCurrentFrame = frames[mCurrentAnimationIndex];
		return mCurrentFrame;
	}

	const Frame& GetCurrentFrame() const
	{
		return mCurrentFrame;
	}

	SDL_RendererFlip GetFlip() const
	{
		return mFlip;
	}

protected:
	phmap::flat_hash_map<uint16, std::vector<Frame>> mAnimations = {};
	uint16 mCurrentState = {}; ///< Enum state
	int32 mCurrentAnimationIndex = 0;
	Frame mCurrentFrame = {}; ///< Put this on the stack as a copy for faster access
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};
