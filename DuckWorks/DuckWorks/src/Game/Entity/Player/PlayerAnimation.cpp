#include "Precomp.h"
#include "Game/Entity/Player/PlayerAnimation.h"

#include "Game/Entity/Player/Player.h"

PlayerAnimation::PlayerAnimation(Player* inPlayer)
	: mPlayer(inPlayer)
{
	for (int i = 0; i < 16; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {32, 32};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 0;
		AddFrame(Cast<uint16>(State::Idle), frame);
	}

	for (int i = 0; i < 64; i++)
	{
		Frame frame;
		frame.mDuration = 0.1f;
		frame.mSize = {8, 8};
		frame.mPosition.x = frame.mSize.x * i;
		frame.mPosition.y = 0;
		AddFrame(Cast<uint16>(State::Walk), frame);
	}
}

void PlayerAnimation::Update(float)
{
	fm::vec2 velocity = mPlayer->GetVelocity();
	if (velocity.length2() > 1.f)
	{
		SetState(static_cast<uint16>(State::Walk));
	}
	else
	{
		SetState(static_cast<uint16>(State::Idle));
	}
}
