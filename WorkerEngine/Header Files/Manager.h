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

	void Init(int num)
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
		while (!_actions.empty() && _poolOfThreads->checkFree())
		{
			if (_poolOfThreads->emplaceJob(_actions.front()))
				_actions.pop_front();
		}
		if (!_actions.empty())
		{
			std::shared_ptr<Job> j = _actions.front();
			_actions.pop_front();
			return j;
		}
		return nullptr;
	}

	bool checkDone()
	{
		return _poolOfThreads->checkWorking() ? true : false;
	}

private:
	Manager() {}
	ThreadPool * _poolOfThreads;
	std::deque<std::shared_ptr<Job>> _actions;
};