#pragma once
#include <Game/World/Entity/Entity.h>

class Player : public DC::RTTIClass
{
	RTTI_CLASS(Player, RTTIClass)
public:


private:
	DC::Ref<Entity> mEntity;
};
