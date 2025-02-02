#pragma once

#include <Core/Utilities/Mutex.h>

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

// A threadsafe-queue.
enum class DequeueResult
{
	Success,
	Empty,
	Locked
};

template<class T>
class SafeQueue
{
public:
	SafeQueue(void)
		: q()
		, m()
		, c()
	{}

	~SafeQueue(void)
	{}

	// Add an element to the queue.
	void Enqueue(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	bool TryEnqueue(T t)
	{
		if (m.try_lock())
		{
			q.push(t);
			c.notify_one();
			m.unlock();
			return true;
		}
		return false;
	}

	// Get the "front"-element.
	// If the queue is empty, wait till a element is available.
	T Dequeue(void)
	{
		std::unique_lock<std::mutex> lock(m);
		while (q.empty())
		{
			// release lock as long as the wait and reacquire it afterward.
			c.wait(lock);
		}
		T val = q.front();
		q.pop();
		return val;
	}

	Pair<DequeueResult, T> TryDequeue()
	{
		if (m.try_lock())
		{
			if (q.empty())
			{
				m.unlock();
				return {DequeueResult::Empty, T()};
			}
			T val = q.front();
			q.pop();
			m.unlock();
			return {DequeueResult::Success, val};
		}
		return {DequeueResult::Locked, T()};
	}

	void Clear()
	{
		std::lock_guard<std::mutex> lock(m);
		while (!q.empty())
		{
			q.pop();
		}
	}

	uint64 Size() const
	{
		return q.size();
	}

	bool IsEmpty() const
	{
		return q.empty();
	}

	void Lock()
	{
		m.lock();
	}

	void Unlock()
	{
		m.unlock();
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
};
