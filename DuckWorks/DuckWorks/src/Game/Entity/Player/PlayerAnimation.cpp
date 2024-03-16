#include "Precomp.h"
#include "Game/Entity/Player/PlayerAnimation.h"

#include "Game/Entity/Player/Player.h"

PlayerAnimation::PlayerAnimation(Player* inPlayer)
	: mPlayer(inPlayer)
{
	CreateFramesParams params;
	params.mState = Cast<uint16>(State::Idle);
	params.mStart = {0, 0};
	params.mSize = {192, 192};
	params.mDuration = 0.1f;
	params.mCount = 6;
	CreateHorizontalFrames(params);

	params.mState = Cast<uint16>(State::Walk);
	params.mStart.y = 192 * 1;
	CreateHorizontalFrames(params);

	params.mState = Cast<uint16>(State::Attack1);
	params.mStart.y = 192 * 2;
	CreateHorizontalFrames(params);

	params.mState = Cast<uint16>(State::Attack2);
	params.mStart.y = 192 * 3;
	CreateHorizontalFrames(params);

	params.mState = Cast<uint16>(State::Attack3);
	params.mStart.y = 192 * 4;
	CreateHorizontalFrames(params);

	params.mState = Cast<uint16>(State::Attack4);
	params.mStart.y = 192 * 5;
	CreateHorizontalFrames(params);
}

static uint32 gSeed = 0;

void PlayerAnimation::Update(float)
{
	fm::vec2 velocity = {};
	if (velocity.x > 0)
		mFlip = SDL_FLIP_NONE;
	else if (velocity.x < 0)
		mFlip = SDL_FLIP_HORIZONTAL;

	if (
		mCurrentState == static_cast<uint16>(State::Attack1) ||
		mCurrentState == static_cast<uint16>(State::Attack2) ||
		mCurrentState == static_cast<uint16>(State::Attack3) ||
		mCurrentState == static_cast<uint16>(State::Attack4))
	{
		// If we're not at frame 0 we want to finish the animation before switching states
		if (mCurrentAnimationIndex != 0)
			return;
	}

	if (mPlayer->IsAttacking())
	{
		gSeed = fm::wang_hash(gSeed);
		switch (gSeed % 4)
		{
		case 0:
			SetState(static_cast<uint16>(State::Attack1));
			break;
		case 1:
			SetState(static_cast<uint16>(State::Attack2));
			break;
		case 2:
			SetState(static_cast<uint16>(State::Attack3));
			break;
		case 3:
			SetState(static_cast<uint16>(State::Attack4));
			break;
		default:
			gAssert(false, "Invalid index");
			SetState(static_cast<uint16>(State::Attack1));
			break;
		}
		return;
	}

	if (velocity.length2() > 10.f)
	{
		SetState(static_cast<uint16>(State::Walk));
	}
	else
	{
		SetState(static_cast<uint16>(State::Idle));
	}
}
