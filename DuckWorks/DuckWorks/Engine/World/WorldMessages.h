#pragma once
#include <Core/RTTI/Message.h>

class World;

struct MsgWorld : public MsgBase
{
	RTTI_MSG()

	World* GetWorld() const { return mWorld; }
private:
	World* mWorld = nullptr;

	friend class World;
};

struct MsgOnAddedToWorld : public MsgWorld
{
	RTTI_MSG()
};