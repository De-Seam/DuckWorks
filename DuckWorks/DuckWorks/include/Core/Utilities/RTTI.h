#pragma once
#define RTTI_CLASS(inClassName, inParentClassName) \
public: \
	using Base = inParentClassName; \
	static const char* sGetClassName() { return #inClassName; } \
	static const char* sGetParentClassName() { return #inParentClassName; } \
	virtual const char* GetClassName() const override { return inClassName::sGetClassName(); } \
	virtual const char* GetParentClassName() const override { return inClassName::sGetParentClassName(); } \
private:

class RTTIBaseClass
{
public:
	virtual const char* GetClassName() const = 0;
	virtual const char* GetParentClassName() const = 0;
};

#define RTTI_STRUCT(inStructName, inParentStructName) \
	using Base = inParentStructName; \
	static const char* sGetClassName() { return #inStructName; } \
	static const char* sGetParentClassName() { return #inParentStructName; } \
	virtual const char* GetClassName() const override { return inStructName::sGetClassName(); } \
	virtual const char* GetParentClassName() const override { return inStructName::sGetParentClassName(); }

struct RTTIBaseStruct
{
	virtual const char* GetClassName() const = 0;
	virtual const char* GetParentClassName() const = 0;
};