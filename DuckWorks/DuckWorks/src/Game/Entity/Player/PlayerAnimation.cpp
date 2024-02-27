#include "Precomp.h"
#include "Game/Entity/Player/PlayerAnimation.h"

#include "Game/Entity/Player/Player.h"

PlayerAnimation::PlayerAnimation(Player* inPlayer)
	: mPlayer(inPlayer)
{
	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 0;
		AddFrame(Cast<uint16>(State::Idle), frame);
	}

	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 192;
		AddFrame(Cast<uint16>(State::Walk), frame);
	}

	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 192 * 2;
		AddFrame(Cast<uint16>(State::Attack1), frame);
	}

	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 192 * 3;
		AddFrame(Cast<uint16>(State::Attack2), frame);
	}

	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 192 * 4;
		AddFrame(Cast<uint16>(State::Attack3), frame);
	}

	for (int i = 0; i < 6; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {192, 192};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 192 * 5;
		AddFrame(Cast<uint16>(State::Attack4), frame);
	}
}

static uint32 gSeed = 0;

void PlayerAnimation::Update(float)
{
	fm::vec2 velocity = mPlayer->GetVelocity();
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
