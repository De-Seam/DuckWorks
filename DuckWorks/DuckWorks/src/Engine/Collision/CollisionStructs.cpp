#include "Precomp.h"
#include "Engine/Collision/CollisionStructs.h"

void to_json(Json& outJson, const AABB& inVariable)
{
	outJson = Json{inVariable.mMin, inVariable.mMax};
}

void from_json(const Json& inJson, AABB& outVariable)
{
	inJson.at(0).get_to(outVariable.mMin);
	inJson.at(1).get_to(outVariable.mMax);
}
