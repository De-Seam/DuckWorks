#include "Precomp.h"
#include "Engine/Timer/TimerManager.h"

TimerManager gTimerManager;

void TimerManager::Update(float inDeltaTime)
{
	mWorldTime += inDeltaTime;

	Timer* handle = mFirstTimerHandle.get();
	UniquePtr<Timer> previous_handle = nullptr; ///< Take ownership of the previous handle
	while (handle != nullptr && handle->mTriggerTime < mWorldTime)
	{
		handle->mFunctionPtr();
		handle = handle->mNext.get();

		// previous_handle basically follows the handle cleaning up pointers.
		if (previous_handle)
		{
			previous_handle = std::move(previous_handle->mNext);
		}
		else
		{
			previous_handle = std::move(mFirstTimerHandle);
		}
	}
}

TimerManager::TimerHandle TimerManager::AddTimer(TimerParams inParams)
{
	gAssert(inParams.mFunctionPtr != nullptr, "Function pointer is null!");
	gAssert(inParams.mDelay >= 0.0f, "Delay is negative!");

	UniquePtr<Timer> new_timer = std::make_unique<Timer>();
	new_timer->mFunctionPtr = inParams.mFunctionPtr;
	new_timer->mTriggerTime = mWorldTime + inParams.mDelay;


	TimerHandle return_handle;
	return_handle.mUID = new_timer->mUID;
	return_handle.mTriggerTime = new_timer->mTriggerTime;

	if (mFirstTimerHandle == nullptr)
	{
		mFirstTimerHandle = std::move(new_timer);
		return return_handle;
	}

	Timer* handle = mFirstTimerHandle.get();
	Timer* previous_handle = nullptr;
	while (handle && new_timer->mTriggerTime < handle->mTriggerTime)
	{
		previous_handle = handle;
		handle = handle->mNext.get();
	}

	if (handle)
		new_timer->mNext = std::move(handle->mNext);

	if (previous_handle)
		previous_handle->mNext = std::move(new_timer);
	else
		mFirstTimerHandle = std::move(new_timer);

	return return_handle;
}

bool TimerManager::RemoveTimer(const TimerHandle& inHandle)
{
	Timer* handle = mFirstTimerHandle.get();
	Timer* previous_handle = nullptr;
	while (handle)
	{
		if (handle->mUID == inHandle.mUID)
		{
			if (previous_handle)
				previous_handle->mNext = std::move(handle->mNext);
			else
				mFirstTimerHandle = std::move(handle->mNext);
			return true;
		}
		// We have passed the time, early out and return false
		else if (handle->mTriggerTime > inHandle.mTriggerTime)
			return false;

		previous_handle = handle;
		handle = handle->mNext.get();
	}
	return false;
}