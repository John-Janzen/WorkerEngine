#pragma once

#include "ThreadPool.h"
class Manager
{
public:
	~Manager() {}

	/*
	* Get the static instance of the Manager
	* Essentially global
	*/
	static Manager& instance()
	{
		static Manager manager;
		return manager;
	}

	void Init(const size_t & num)
	{
		_poolOfThreads = new ThreadPool(num);
	}

	/*
	* Close the Manager
	* Deallocate the systems
	*/
	void Close()
	{
		delete(_poolOfThreads);
	}

	bool checkThreads()
	{
		return _poolOfThreads->checkBusy() ? true : false;
	}

	void trasferList(std::deque<std::shared_ptr<Job>> & list)
	{
		for (int i = 0; i < list.size(); i++)
			_actions.emplace_back(std::move(list[i]));
	}
	
	/*
	* Give Job to the thread
	*/
	std::shared_ptr<Job> AllocateJobs()
	{
		std::shared_ptr<Job> j = nullptr;
		while (!_actions.empty() && _poolOfThreads->checkFree())
		{
			if (_actions.front()->Get_PriorityThread() != WHICH_THREAD::MAIN_ONLY)
			{
				if (_poolOfThreads->emplaceJob(std::move(_actions.front())))
					_actions.pop_front();
			}
			else if (_actions.front()->Get_PriorityThread() == WHICH_THREAD::MAIN_ONLY && j == nullptr)
			{
				j = std::move(_actions.front());
				_actions.pop_front();
			}
			else
			{
				return j;
			}
		}
		if (!_actions.empty() && j == nullptr)
		{
			j = std::move(_actions.front());
			_actions.pop_front();
		}
		return j;
	}

	bool checkDone()
	{
		return (_actions.empty() && _poolOfThreads->DoneWorking()) ? true : false;
	}

private:
	Manager() {}
	ThreadPool * _poolOfThreads;
	std::deque<std::shared_ptr<Job>> _actions;
};