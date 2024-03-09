#pragma once

#pragma warning( push )
#pragma warning( disable : 4099) // First seen using 'class' now seen using 'struct'

#define RTTI_CLASS(inClassName, inParentClassName) \
public: \
	using Base = inParentClassName; \
	static const char* sGetClassName() { return #inClassName; } \
	static const char* sGetParentClassName() { return #inParentClassName; } \
	virtual const char* GetClassName() const override { return inClassName::sGetClassName(); } \
	virtual const char* GetParentClassName() const override { return inClassName::sGetParentClassName(); } \
	virtual Json Serialize() const override; \
	virtual void Deserialize(const Json& inJson) override;

#define RTTI_EMPTY_SERIALIZE_DEFINITION(inClassName) \
	Json inClassName::Serialize() const { return Base::Serialize(); } \
	void inClassName::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

class RTTIBaseClass
{
public:
	virtual const char* GetClassName() const = 0;
	virtual const char* GetParentClassName() const = 0;

	virtual Json Serialize() const { return {}; }
	virtual void Deserialize(const Json& inJson) { (void)inJson; }
};

#define REGISTER_ENTITY(inEntity) \
	gEntityFactory.RegisterClass<inEntity>(#inEntity)

#define REGISTER_COMPONENT(inComponent) \
	gComponentFactory.RegisterClass<inComponent>(#inComponent)

#define REGISTER_DEBUG_UI_WINDOW(inWindow) \
	gDebugUIWindowFactory.RegisterClass<inWindow>(#inWindow)
