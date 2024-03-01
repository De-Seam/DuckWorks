#include "Precomp.h"
#include "Engine/Timer/TimerManager.h"

TimerManager gTimerManager;

void TimerManager::Update(float inDeltaTime)
{
	OPTICK_EVENT("TimerManager::Update");

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
			HandleTimerLooping(previous_handle.get());
		}
		else
		{
			previous_handle = std::move(mFirstTimerHandle);
			HandleTimerLooping(previous_handle.get());
		}
	}
}

TimerManager::TimerHandle TimerManager::AddTimer(TimerParams inParams)
{
	gAssert(inParams.mFunctionPtr != nullptr, "Function pointer is null!");
	gAssert(inParams.mDelay >= 0.0f, "Delay is negative!");

	// Make timer pointer on heap
	UniquePtr<Timer> new_timer = std::make_unique<Timer>();
	new_timer->mFunctionPtr = inParams.mFunctionPtr;
	new_timer->mTriggerTime = mWorldTime + inParams.mDelay;
	new_timer->mDelay = inParams.mLoop ? inParams.mDelay : 0;

	// Create the return handle
	TimerHandle return_handle;
	return_handle.mUID = new_timer->mUID;
	return_handle.mTriggerTime = new_timer->mTriggerTime;

	// Check if this should be the first timer in the list and early out
	if (mFirstTimerHandle == nullptr)
	{
		mFirstTimerHandle = std::move(new_timer);
		return return_handle;
	}

	// Find the correct place to insert the timer
	Timer* handle = mFirstTimerHandle.get();
	Timer* previous_handle = nullptr;
	while (handle && new_timer->mTriggerTime < handle->mTriggerTime)
	{
		previous_handle = handle;
		handle = handle->mNext.get();
	}

	// Set the handles
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
	// Find the timer handle
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
		if (handle->mTriggerTime > inHandle.mTriggerTime)
			return false;

		previous_handle = handle;
		handle = handle->mNext.get();
	}
	// Timer not found. It probably expired already
	return false;
}

void TimerManager::HandleTimerLooping(Timer* inTimer)
{
	// Early out if the timer is not looping
	if (inTimer->mDelay < 0.0f)
		return;

	TimerParams params;
	params.mDelay = inTimer->mDelay;
	params.mFunctionPtr = inTimer->mFunctionPtr;
	params.mLoop = true;
	AddTimer(params);
}
