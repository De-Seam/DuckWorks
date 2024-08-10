#include <Precomp.h>
#include <Core/Math/Transform.h>

void to_json(Json& outJson, const Transform2D& inVariable) 
{
	outJson["mPosition"] = inVariable.mPosition;
	outJson["mScale"] = inVariable.mScale;
	outJson["mRotation"] = inVariable.mRotation;
}

void from_json(const Json& inJson, Transform2D& outVariable) 
{
	outVariable.mPosition = inJson["mPosition"];
	outVariable.mScale = inJson["mScale"];
	outVariable.mRotation = inJson["mRotation"];
}
