#include "Precomp.h"
#include "Engine/Threads/ThreadManager.h"
#include <atomic>
#include <condition_variable>
#include <vector>

RTTI_CLASS_DEFINITION(ThreadManager)
RTTI_EMPTY_SERIALIZE_DEFINITION(ThreadManager)

ThreadManager gThreadManager;

void ThreadTask::WaitUntilCompleted() 
{
	std::unique_lock<std::mutex> lock(mCompletedMutex);
	if (mCompleted)
		return;
	mCompletedConditionVariable.wait(lock, [this] { return mCompleted; });
}

ThreadManager::ThreadManager() {}

void ThreadManager::Init()
{
	uint32 threadCount = std::thread::hardware_concurrency() - 1;
	mRunning = true;
	for (uint32 i = 0; i < threadCount; ++i)
	{
		mThreads.emplace_back(std::thread([&](int32 inIndex)
		{
			WorkerThread(inIndex);
		}, i));
	}
}

void ThreadManager::Shutdown()
{
	mRunning = false;
	mThreadConditionVariable.notify_all();
	// Join all threads
	for (auto& thread : mThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	mThreads.clear();
}

void ThreadManager::AddTask(const SharedPtr<ThreadTask>& inTask, ThreadPriority inPriority)
{
	inTask->mPriority = inPriority;
	{
		std::unique_lock<std::mutex> lock(mTaskMutex);
		mTasks[SCast<uint64>(inPriority)].Enqueue(inTask);
	}
	mThreadConditionVariable.notify_one();
}

void ThreadManager::WaitUntilPriorityEmpty(ThreadPriority inPriority)
{
	std::unique_lock<std::mutex> lock(mPriorityEmptyMutex[SCast<uint64>(inPriority)]);
	mPriorityEmptyCV[SCast<uint64>(inPriority)].wait(lock, [this, inPriority]
	{
		return mTasks[SCast<uint64>(inPriority)].IsEmpty();
	});
}

void ThreadManager::WorkerThread(int32 inIndex)
{
	OPTICK_THREAD("ThreadManager::WorkerThread")
	while (mRunning)
	{
		{
			PROFILE_SCOPE(ThreadManager::WaitForTask)
			std::unique_lock<std::mutex> lock(mTaskMutex);

			mThreadConditionVariable.wait(lock,[this]
			{
				for (int32 i = SCast<int32>(ThreadPriority::VeryHigh); i >= 0; i--)
				{
					if (!mTasks[i].IsEmpty())
						return true;
				}
				return !mRunning;
			});
		}
		for (int32 i = SCast<int32>(ThreadPriority::VeryHigh); i >= 0; i--)
		{
			SafeQueue<SharedPtr<ThreadTask>>& tasks_queue = mTasks[i];
			Pair<DequeueResult, SharedPtr<ThreadTask>> item = tasks_queue.TryDequeue();

			if (item.first == DequeueResult::Empty)
				continue;

			if (item.first == DequeueResult::Locked)
				break;

			PROFILE_SCOPE(ThreadManager::ExecuteTask)
			item.second->Execute();
			OnTaskCompleted(item.second);
		}
	}
}

void ThreadManager::OnTaskCompleted(SharedPtr<ThreadTask> &inTask)
{
	PROFILE_SCOPE(ThreadManager::OnTaskCompleted)
	{
		std::unique_lock<std::mutex> lock(inTask->mCompletedMutex);
		inTask->mCompleted = true;
	}
	inTask->mCompletedConditionVariable.notify_all();
	const uint64 index = SCast<uint64>(inTask->mPriority);
	if (mTasks[index].IsEmpty())
	{
		std::lock_guard<std::mutex> lock(mPriorityEmptyMutex[index]); // Lock if needed
		mPriorityEmptyCV[index].notify_all();
	}
}
