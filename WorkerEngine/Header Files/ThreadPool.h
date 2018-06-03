#pragma once
#include <vector>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <mutex>

#include "ThreadWorker.h"

class ThreadPool
{
public:

	ThreadPool(int num) 
	{
		for (int i = 0; i < num; i++)
			_workers.emplace_back(new ThreadWorker(i, this));
		count = max = num;
	}

	~ThreadPool() 
	{
		while (checkBusy()) {}
		for (ThreadWorker * thread : _workers)
			delete(thread);
	}

	bool emplaceJob(std::shared_ptr<Job> job)
	{
		if (_workers.empty()) return false;
		else
		{
			std::unique_lock<std::mutex> lock(_lockMutex);
			ThreadWorker * t = _workers.front();
			_workers.pop_front();
			count--;
			t->a = std::move(job);
			_c.notify_all();
			return true;
		}
	}

	/*
	* Signal manager that the thread is done
	* Increment the count
	*/
	void signalDone(ThreadWorker * tw) 
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		_workers.emplace_back(tw);
		count++;
		_c.notify_one();
	}

	/*
	* Check if all the threads are busy or not
	* For Synchronization
	*/
	bool checkBusy()
	{
		return (_workers.size() < max) ? true : false;
	}

	bool checkFree()
	{
		return (_workers.size() > 0) ? true : false;
	}

	bool DoneWorking()
	{
		return (_workers.size() == max) ? true : false;
	}

private:
	std::atomic<int> count, max;
	std::deque<ThreadWorker*> _workers;
	std::mutex _lockMutex;
	std::condition_variable _c;
};
