#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>

using ManagerTypeID = TypeID<class Manager>;

#define RTTI_MANAGER(inClassName, inBaseClassName) \
	RTTI_CLASS(inClassName, inBaseClassName) \
public: \
	static const ManagerTypeID& sGetManagerTypeID() { return sManagerTypeID; }; \
\
private: \
	inline static ManagerTypeID sManagerTypeID = {};

class Manager : public RTTIClass
{
	RTTI_CLASS(Manager, RTTIClass)

public:
	struct Settings
	{
		bool mWantsUpdate = false;

		template<typename taType>
		void AddInitDependency() { mInitDependencies.emplace_back(taType::sGetRTTI()); }
		template<typename taType>
		void AddShutdownDependency() { mShutdownDependencies.emplace_back(taType::sGetRTTI()); }
		template<typename taType>
		void AddUpdateDependency() { mUpdateDependencies.emplace_back(taType::sGetRTTI()); }

		const Array<RTTI*>& GetInitDependencies() const { return mInitDependencies; }
		const Array<RTTI*>& GetShutdownDependencies() const { return mShutdownDependencies; }
		const Array<RTTI*>& GetUpdateDependencies() const { return mUpdateDependencies; }

	private:
		Array<RTTI*> mInitDependencies;
		Array<RTTI*> mShutdownDependencies;
		Array<RTTI*> mUpdateDependencies;
	};

	const Settings& GetSettings() const { return mSettings; }

	virtual void Init() {}
	virtual void Shutdown() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

protected:
	// Settings should be set in the constructor
	Settings mSettings;
};
