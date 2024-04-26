#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/UID.h"

class TimerManager
{
public:
	void Update(float inDeltaTime); ///< Increments world time

	// Handle to a timer
	struct TimerHandle
	{
		double mTriggerTime; ///< Added this variable in case we want to remove a timer that doesn't exist so we don't have to traverse the whole linked list
		UID mUID;
	};

	struct TimerParams
	{
		std::function<void()> mFunctionPtr = nullptr;
		float mDelay = 0.1f; ///< 0 for next frame
		bool mLoop = false;
	};

	TimerHandle AddTimer(const TimerParams& inParams);

	bool RemoveTimer(const TimerHandle& inHandle);

private:
	// Timer is a linked list.
	// It does not loop around, so it is not a circular linked list.
	struct Timer
	{
		std::function<void()> mFunctionPtr = nullptr;
		double mTriggerTime = 0.0;
		UID mUID = {};
		UniquePtr<Timer> mNext = nullptr;
		float mDelay = -1.f; ////< -1 means it's not a loop timer
	};

	UniquePtr<Timer> mFirstTimerHandle; ///< Beginning of linked list
	double mWorldTime = 0.0;

private:
	void HandleTimerLooping(Timer* inTimer);
};

extern TimerManager gTimerManager;
