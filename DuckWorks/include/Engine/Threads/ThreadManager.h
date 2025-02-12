#pragma once
#include "Core/CoreBase.h"
#include "Core/Manager/Manager.h"
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
		mCompleted = false;
	}

	virtual void Execute() = 0;

	bool IsCompleted() const { return mCompleted; }
	void WaitUntilCompleted();

private:
	std::atomic<bool> mCompleted = false;
	ThreadPriority mPriority = ThreadPriority::Normal;

	std::condition_variable mCompletedConditionVariable;
	std::mutex mCompletedMutex;

	friend class ThreadManager;
};

class ThreadManager : public Manager
{
	RTTI_CLASS(ThreadManager, Manager, StandardAllocator)
public:
	struct ConstructParameters : public Base::ConstructParameters {};

	ThreadManager(const ConstructParameters& inConstructParameters = {});

	virtual void Init() override;
	virtual void Shutdown() override;
	void AddTask(const SharedPtr<ThreadTask>& inTask, ThreadPriority inPriority);

	bool IsPriorityEmpty(ThreadPriority inPriority) const { return mTasks[SCast<uint64>(inPriority)].IsEmpty(); }
	void WaitUntilPriorityEmpty(ThreadPriority inPriority);

private:
	// mTasks[mPriority] contains all tasks that are of that priority.
	SafeQueue<SharedPtr<ThreadTask>> mTasks[SCast<uint64>(ThreadPriority::VeryHigh) + 1] = {};
	std::mutex mTaskMutex;

	Array<std::thread> mThreads = {};

	std::atomic_bool mRunning = false;

	std::condition_variable mThreadConditionVariable;

	std::condition_variable mPriorityEmptyCV[SCast<uint64>(ThreadPriority::VeryHigh) + 1];
	std::mutex mPriorityEmptyMutex[SCast<uint64>(ThreadPriority::VeryHigh) + 1]; // If needed

private:
	void WorkerThread(int32 inIndex);
	void OnTaskCompleted(SharedPtr<ThreadTask>& inTask);
};

// Gets the current Thread Index. 0 is the main thread.
int32 gGetCurrentThreadIndex();
bool gIsMainThread();
extern ThreadManager gThreadManager;
