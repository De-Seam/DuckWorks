#include "Precomp.h"
#include "Engine/Renderer/AnimationBase.h"

void AnimationBase::AddAnimation(uint16 inState, const std::vector<Frame>& inFrames)
{
	mAnimations[inState] = inFrames;
}

void AnimationBase::AddFrame(uint16 inState, const Frame& inFrame)
{
	mAnimations[inState].push_back(inFrame);
}

void AnimationBase::CreateHorizontalFrames(const CreateFramesParams& inParams)
{
	for (int32 i = 0; i < inParams.mCount; i++)
	{
		Frame frame;
		frame.mDuration = inParams.mDuration;
		frame.mSize = inParams.mSize;
		frame.mPosition.x = inParams.mStart.x + i * inParams.mSize.x;
		frame.mPosition.y = inParams.mStart.y;
		AddFrame(inParams.mState, frame);
	}
}

void AnimationBase::CreateVerticalFrames(const CreateFramesParams& inParams)
{
	for (int32 i = 0; i < inParams.mCount; i++)
	{
		Frame frame;
		frame.mDuration = inParams.mDuration;
		frame.mSize = inParams.mSize;
		frame.mPosition.x = inParams.mStart.x;
		frame.mPosition.y = inParams.mStart.y + i * inParams.mSize.y;
		AddFrame(inParams.mState, frame);
	}
}

void AnimationBase::SetState(uint16 inState, int32 inFrame)
{
	if (mCurrentState == inState)
		return;

	gAssert(mAnimations.find(inState) != mAnimations.end(), "Animation state was not set!");
	gAssert(inFrame < mAnimations[inState].size(), "inFrame was invalid!");
	mCurrentState = inState;
	mCurrentAnimationIndex = inFrame;
	mCurrentFrame = mAnimations[inState][inFrame];
	mCurrentFrame.mFunctionPtr();
}

void AnimationBase::SetFrame(uint16 inState, int32 inIndex, const Frame& inFrame)
{
	gAssert(mAnimations.find(inState) != mAnimations.end(), "Animation state was not set!");
	gAssert(inIndex < mAnimations[inState].size(), "inIndex was invalid!");
	mAnimations[inState][inIndex] = inFrame;
}

const AnimationBase::Frame& AnimationBase::IncrementFrame()
{
	std::vector<Frame>& frames = mAnimations[mCurrentState];
	mCurrentAnimationIndex = (mCurrentAnimationIndex + 1) % frames.size();
	mCurrentFrame = frames[mCurrentAnimationIndex];
	mCurrentFrame.mFunctionPtr();
	return mCurrentFrame;
}

const AnimationBase::Frame& AnimationBase::GetCurrentFrame() const
{
	return mCurrentFrame;
}

const AnimationBase::Frame& AnimationBase::GetFrame(uint16 inState, int32 inIndex) const
{
	std::vector<AnimationBase::Frame>& frames = mAnimations[inState];
	return frames[inIndex];
}

SDL_RendererFlip AnimationBase::GetFlip() const
{
	return mFlip;
}
