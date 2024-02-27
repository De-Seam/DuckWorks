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

	void AddAnimation(uint16 inState, const std::vector<Frame>& inFrames);
	void AddFrame(uint16 inState, const Frame& inFrame);

	struct CreateFramesParams
	{
		uint16 mState = {};
		fm::ivec2 mStart = {};
		fm::ivec2 mSize = {};
		float mDuration = 0.1f;
		int32 mCount = 0;
	};

	void CreateHorizontalFrames(const CreateFramesParams& inParams);
	void CreateVerticalFrames(const CreateFramesParams& inParams);

	void SetState(uint16 inState, int32 inFrame = 0);

	const Frame& IncrementFrame();
	const Frame& GetCurrentFrame() const;
	SDL_RendererFlip GetFlip() const;

protected:
	phmap::flat_hash_map<uint16, std::vector<Frame>> mAnimations = {};
	uint16 mCurrentState = {}; ///< Enum state
	int32 mCurrentAnimationIndex = 0;
	Frame mCurrentFrame = {}; ///< Put this on the stack as a copy for faster access
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};
