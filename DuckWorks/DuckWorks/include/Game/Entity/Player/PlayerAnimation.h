#pragma once
// Engine includes
#include "Engine/Renderer/AnimationBase.h"

class Player;

class PlayerAnimation : public AnimationBase
{
public:
	PlayerAnimation(Player* inPlayer);

	virtual void Update(float inDeltaTime) override;

protected:
	enum class State : uint16
	{
		Idle,
		Walk,
		Attack1,
		Attack2,
		Attack3,
		Attack4
	};

	Player* mPlayer;
};
