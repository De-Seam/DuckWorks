#include <Precomp.h>
#include <Core/Math/Vector.h>

void to_json(Json& outJson, const IVec2& inVariable) 
{
	outJson = { inVariable.mX, inVariable.mY };
}

void from_json(const Json& inJson, IVec2& outVariable) 
{
	outVariable.mX = inJson[0];
	outVariable.mY = inJson[1];
}

void to_json(Json& outJson, const Vec2& inVariable) 
{
	outJson = { inVariable.mX, inVariable.mY };
}

void from_json(const Json& inJson, Vec2& outVariable) 
{
	outVariable.mX = inJson[0];
	outVariable.mY = inJson[1];
}
