#pragma once
#define RTTI_CLASS(inClassName, inParentClassName) \
public: \
	using Base = inParentClassName; \
	static const char* sGetClassName() { return #inClassName; } \
	static const char* sGetParentClassName() { return #inParentClassName; } \
	virtual const char* GetClassName() const override { return inClassName::sGetClassName(); } \
	virtual const char* GetParentClassName() const override { return inClassName::sGetParentClassName(); }

class RTTIBaseClass
{
public:
	virtual const char* GetClassName() const = 0;
	virtual const char* GetParentClassName() const = 0;
};

#define RTTI_STRUCT(inClassName) \
public: \
	static const char* sGetClassName() { return #inClassName; } \
	virtual const char* GetClassName() const override { return inClassName::sGetClassName(); }

#define REGISTER_ENTITY(inEntity) \
	gEntityFactory.RegisterClass<inEntity>(#inEntity)

#define REGISTER_COMPONENT(inComponent) \
	gComponentFactory.RegisterClass<inComponent>(#inComponent)

#define REGISTER_DEBUG_UI_WINDOW(inWindow) \
	gDebugUIWindowFactory.RegisterClass<inWindow>(#inWindow)
