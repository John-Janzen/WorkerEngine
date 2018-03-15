#pragma once
#include <queue>
#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class Locker
{
public:
	~Locker<T>() {}
	Locker<T>() {}
	
	void enqueue(T t)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		q.push_back(t);
		c.notify_one();
	}

	T dequeue(void)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		while (q.empty())
			c.wait(lock);

		T val = q.front();
		q.pop_front();
		return val;
	}

	bool empty()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		bool em = q.empty() ? true : false;
		c.notify_one();
		return em;
	}

	bool contains(T t)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		for (T item : q)
			if (t == item) 
			{
				c.notify_one();
				return true;
			}
		c.notify_one();
		return false;
	}

	void Lock() { _lockMutex.lock(); }
	void Unlock() { _lockMutex.unlock(); }

	std::list<T> Get_List() { return q; }
private:
	std::list<T> q;
	std::mutex _lockMutex;
	std::condition_variable c;
};
