#pragma once
#include <mutex>
#include <condition_variable>
#include <map>

#include "Locker.h"
#include "ThreadWorker.h"

class Manager
{
public:
	~Manager() {}

	static Manager& instance()
	{
		static Manager manager;
		return manager;
	}

	void Close()
	{
		for (std::map<std::string, System*>::iterator i = _systems.begin(); i != _systems.end(); ++i)
		{
			i->second->Close();
			delete(i->second);
		}
		_systems.clear();
	}

	void addJob(std::shared_ptr<Job> j)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		if (!checkHasSystem(j->Get_System(), j->Get_JobType()))
		{
			_actions.enqueue(j);
		}
		_c.notify_all();
	}

	void addJob(std::string name, JOB_TYPES j = JOB_TYPES::SYSTEM_DEFAULT, void* ptr = nullptr)
	{
		addJob(std::make_shared<Job>(_systems[name], j, ptr));
	}

	void addSystem(std::string key, System * s)
	{
		_systems.emplace(std::make_pair(key, s));
	}
	
	void Give_Job(ThreadWorker * t)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		t->a = _actions.dequeue();
		_c.notify_one();
	}

	bool contains(std::shared_ptr<Job> j)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		_actions.contains(j);
		_c.notify_all();
	}

	bool checkHasSystem(System* s, JOB_TYPES T)
	{
		for (std::shared_ptr<Job> a : _actions.Get_List())
			if (a->Get_System() == s && a->Get_JobType() == T)
				return true;
		return false;
	}

	bool hasJobs()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		bool empty = (_actions.Get_List().empty()) ? false : true;
		_c.notify_all();
		return empty;
	}

	void passCount(int i) { count = max = i; }

	void signalWorking()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		count--;
		_c.notify_all();
	}

	void signalDone()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		count++;
		_c.notify_all();
	}

	bool checkBusy()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		bool check;
		if (count < max)
			check = true;
		else
			check = false;
		_c.notify_all();
		return check;
	}

private:
	Manager() {}
	int count, max;
	Locker<std::shared_ptr<Job>> _actions;
	std::map<std::string, System*> _systems;
	std::mutex _lockMutex;
	std::condition_variable _c;
};