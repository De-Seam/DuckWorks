#include "Precomp.h"
#include "Engine/Engine/Engine.h"

// Engine includes
#include "Engine/Entity/Entity.h"

Engine gEngine;

void Engine::Init()
{
	mLua.open_libraries(sol::lib::base);

	mLua.new_usertype<fm::vec2>("vec2",
								sol::constructors<fm::vec2(), fm::vec2(float, float)>(),
								"x", &fm::vec2::x,
								"y", &fm::vec2::y);

	mLua.new_usertype<fm::Transform>("Transform",
									sol::constructors<fm::Transform(), fm::Transform(fm::vec2, fm::vec2, float)>(),
									"position", &fm::Transform::position,
									"halfSize", &fm::Transform::halfSize,
									"rotation", &fm::Transform::rotation);

	mLua.new_usertype<Entity>("Entity",
							"GetTransform", &Entity::GetTransform,
							"SetTransform", &Entity::SetTransform);

	mLua["print"] = [](const String& inMessage)
	{
		gLog(ELogType::Info, inMessage.c_str());
	};
}

void Engine::Shutdown()
{
	mLua.collect_garbage();
}

bool Engine::FileExists(const char* inFilePath) const
{
	struct stat buffer;
	return (stat(inFilePath, &buffer) == 0);
}
