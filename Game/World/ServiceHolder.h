#pragma once
#include <DuckCore/Containers/HashMap.h>

#include <Game/World/Service.h>

namespace DC
{
class RTTI;
}

// ServiceHolder holds services, used by the World and Scene.
template<typename taServiceBaseType>
class ServiceHolder
{
public:
	template<typename taServiceType>
	taServiceType* CreateAndAddService();

	void AddService(Service* aService);

	template<typename taServiceType>
	taServiceType& GetService();
	template<typename taServiceType>
	const taServiceType& GetService() const { return const_cast<ServiceHolder*>(this)->GetService<taServiceType>(); }

	template<typename taServiceType>
	taServiceType* FindService();
	template<typename taServiceType>
	const taServiceType* FindService() const { return const_cast<ServiceHolder*>(this)->FindService<taServiceType>(); }

	template<typename taServiceType>
	bool HasService() { return mRTTIToService.Contains(&taServiceType::sGetRTTI()); }

private:
	DC::HashMap<const DC::RTTI*, DC::Ref<Service>> mRTTIToService;
};

template<typename taServiceBaseType>
template<typename taServiceType>
taServiceType* ServiceHolder<taServiceBaseType>::CreateAndAddService()
{
	static_assert(std::is_base_of_v<taServiceType, taServiceBaseType>);

	taServiceType* service = new taServiceType();
	AddService(service);
	return service;
}

template<typename taServiceBaseType>
void ServiceHolder<taServiceBaseType>::AddService(Service* aService)
{
	gAssert(aService->IsA<taServiceBaseType>());
	mRTTIToService[&aService->GetRTTI()] = aService;
}

template<typename taServiceBaseType>
template<typename taServiceType>
taServiceType& ServiceHolder<taServiceBaseType>::GetService()
{
	static_assert(std::is_base_of_v<taServiceType, taServiceBaseType>);

	Service* service = mRTTIToService.At(&taServiceType::sGetRTTI());
	gAssert(service != nullptr);

	return *static_cast<taServiceType*>(service);
}

template<typename taServiceBaseType>
template<typename taServiceType>
taServiceType* ServiceHolder<taServiceBaseType>::FindService()
{
	DC::HashMap<const DC::RTTI*, DC::Ref<Service>>::Iterator iter = mRTTIToService.Find(&taServiceType::sGetRTTI());
	if (!iter.IsValid())
		return nullptr;

	return static_cast<taServiceType*>(iter.GetValue().Get());
}
