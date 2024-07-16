#pragma once
#include "BaseResource.h"

namespace sol
{
class state;
}

struct LuaResource : public BaseResource
{
	RTTI_CLASS(LuaResource, BaseResource, ClassAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	LuaResource(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual uint64 GetMemorySize() const override;

	void RunScript(sol::state& inLua) const;

	const String& GetScript() const { return mScript; }

protected:
	virtual void LoadFromFile(const String& inFile) override;

private:
	String mScript;
};

void from_json(const Json& inJson, SharedPtr<LuaResource>& outVariable);
