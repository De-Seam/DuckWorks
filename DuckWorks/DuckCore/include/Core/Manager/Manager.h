#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/RTTI/RTTIRefClass.h>

using ManagerTypeID = TypeID<class Manager>;

#define RTTI_MANAGER(inClassName, inBaseClassName) \
	RTTI_CLASS(inClassName, inBaseClassName) \
public: \
	static const ManagerTypeID& sGetManagerTypeID() { return sManagerTypeID; }; \
	virtual const ManagerTypeID& GetManagerTypeID() const override { return sManagerTypeID; } \
\
private: \
	inline static ManagerTypeID sManagerTypeID = {};

class Manager : public RTTIRefObject
{
	RTTI_VIRTUAL_CLASS(Manager, RTTIClass)

public:
	struct ManagerSettings
	{
		bool mWantsUpdate = false;

		template<typename taType>
		void AddInitDependency() { mInitDependencies.emplace_back(&taType::sGetRTTI()); }
		template<typename taType>
		void AddShutdownDependency() { mShutdownDependencies.emplace_back(&taType::sGetRTTI()); }
		template<typename taType>
		void AddUpdateDependency() { mUpdateDependencies.emplace_back(&taType::sGetRTTI()); }

		const Array<const RTTI*>& GetInitDependencies() const { return mInitDependencies; }
		const Array<const RTTI*>& GetShutdownDependencies() const { return mShutdownDependencies; }
		const Array<const RTTI*>& GetUpdateDependencies() const { return mUpdateDependencies; }

	private:
		Array<const RTTI*> mInitDependencies;
		Array<const RTTI*> mShutdownDependencies;
		Array<const RTTI*> mUpdateDependencies;
	};

	virtual const ManagerTypeID& GetManagerTypeID() const = 0;

	const ManagerSettings& GetSettings() const { return mManagerSettings; }

	virtual void Init() {}
	virtual void Shutdown() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

protected:
	// Settings should be set in the constructor
	ManagerSettings mManagerSettings;
};
