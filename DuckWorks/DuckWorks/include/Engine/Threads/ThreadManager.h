#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/Mutex.h"
#include "Core/Utilities/SafeQueue.h"

enum class ThreadPriority : uint8
{
	// Very Low priority. Use this for background tasks that don't need to be done quickly. They may take multiple frames to process.
	VeryLow,
	// Low priority. Use this for tasks that don't need to be done quickly but should be done before the frame ends.
	Low,
	// Normal priority. Use this for tasks that should be done before the next frame.
	Normal,
	// High priority. Use this for tasks that should be done as soon as possible.
	High,
	// Very High priority. Use this for tasks that should be done immediately.
	VeryHigh
};

// ThreadTask should be overwritten with the specific code execution to be executed.
class ThreadTask
{
public:
	// Reset the thread task to its default values for reuse.
	virtual void Reset()
	{
		mIsDone = false;
	}

	virtual void Execute() = 0;

	bool IsDone() const { return mIsDone; }

private:
	bool mIsDone = false;
	ThreadPriority mPriority = ThreadPriority::Normal;

	friend class ThreadManager;
};

class ThreadManager : public RTTIBaseClass
{
	RTTI_CLASS(ThreadManager, RTTIBaseClass)
public:
	ThreadManager();

	void Init();
	void Shutdown();
	void AddTask(const SharedPtr<ThreadTask>& inTask, ThreadPriority inPriority);

	bool IsPriorityEmpty(ThreadPriority inPriority) const { return mTasks[SCast<uint64>(inPriority)].IsEmpty(); }
	void WaitUntilPriorityEmpty(ThreadPriority inPriority);

private:
	// mTasks[mPriority] contains all tasks that are of that priority.
	SafeQueue<SharedPtr<ThreadTask>> mTasks[SCast<uint64>(ThreadPriority::VeryHigh) + 1] = {};

	Array<std::thread> mThreads = {};

	std::atomic_bool mRunning = false;

	std::condition_variable mThreadConditionVariable;

	std::condition_variable mPriorityEmptyCV[SCast<uint64>(ThreadPriority::VeryHigh) + 1];
	std::mutex mPriorityEmptyMutex[SCast<uint64>(ThreadPriority::VeryHigh) + 1]; // If needed

private:
	void WorkerThread(int32 inIndex);
	void OnTaskFinished(SharedPtr<ThreadTask>& inTask);
};

extern ThreadManager gThreadManager;