#pragma once

#include <deque>
#include <mutex>

namespace Lamp
{
	template<typename T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue() = default;
		ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
		virtual ~ThreadSafeQueue() { Clear(); }

		const T& Front()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.front();
		}

		void PushBack(const T& item)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.emplace_back(std::move(item));
		}

		bool Empty()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.empty();
		}

		size_t Count()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.size();
		}

		void Clear()
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.clear();
		}

		bool PopFront(T& t)
		{
			std::scoped_lock lock(m_Mutex);
			t = std::move(m_Queue.front());
			m_Queue.pop_front();
			return true;
		}

		bool PopBack(T& t)
		{
			std::scoped_lock lock(m_Mutex);
			t = std::move(m_Queue.back());
			m_Queue.pop_back();
			return true;
		}

	protected:
		std::mutex m_Mutex;
		std::deque<T> m_Queue;
	};
}