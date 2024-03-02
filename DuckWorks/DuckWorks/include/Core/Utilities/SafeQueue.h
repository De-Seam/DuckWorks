#pragma once

#include <Core/Utilities/Mutex.h>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

// A threadsafe-queue.
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
	void enqueue(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	bool try_enqueue(T t)
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
	T dequeue(void)
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

	enum class DequeueResult
	{
		Success,
		Empty,
		Locked
	};

	std::pair<DequeueResult, T> try_dequeue()
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

	void clear()
	{
		std::lock_guard<std::mutex> lock(m);
		while (!q.empty())
		{
			q.pop();
		}
	}

	uint64 size() const
	{
		return q.size();
	}

	bool empty() const
	{
		return q.empty();
	}

	void lock()
	{
		m.lock();
	}

	void unlock()
	{
		m.unlock();
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
};
