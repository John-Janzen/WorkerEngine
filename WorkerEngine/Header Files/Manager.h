#pragma once

#include "Locker.h"
#include "ThreadWorker.h"

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

	/*
	* Close the Manager
	* Deallocate the systems
	*/
	void Close()
	{
		for (std::map<std::string, System*>::iterator i = _systems.begin(); i != _systems.end(); ++i)
			i->second->Close();

		_systems.clear();
	}

	/*
	* Add a job to the list
	*/
	void addJob(std::shared_ptr<Job> j)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		if (!checkHasSystem(j->Get_System(), j->Get_JobType(), j->Get_Data()))
		{
			_actions.enqueue(j);
		}
		_c.notify_all();
	}

	/*
	* Add a job to the list by:
	* Name of system
	* Job type
	* Any void pointer
	*/
	void addJob(std::string name, JOB_TYPES j = JOB_TYPES::SYSTEM_DEFAULT, BaseContent * ptr = nullptr)
	{
		addJob(std::make_shared<Job>(_systems[name], j, ptr));
	}

	/*
	* Add a system to the map
	*/
	void addSystem(std::string key, System * s)
	{
		_systems.emplace(std::make_pair(key, s));
	}
	
	/*
	* Give Job to the thread
	*/
	void Give_Job(ThreadWorker * t)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		t->a = _actions.dequeue();
		_c.notify_one();
	}

	/*
	* Check if job is contained in the list
	*/
	bool contains(std::shared_ptr<Job> j)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		_actions.contains(j);
		_c.notify_all();
	}

	/*
	* Check if the list has the system and job already on it
	*/
	bool checkHasSystem(System* s, JOB_TYPES T, BaseContent * bc = nullptr)
	{
		for (std::shared_ptr<Job> a : _actions.Get_List())
			if (a->Get_System() == s && a->Get_JobType() == T && a->Get_Data() == bc)
				return true;
		return false;
	}

	/*
	* Check if the list has any jobs
	*/
	bool hasJobs()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		bool empty = (_actions.Get_List().empty()) ? false : true;
		_c.notify_all();
		return empty;
	}

	/*
	* Get the number of threads
	*/
	void passCount(int i) { count = max = i; }

	/*
	* Signal manager that the thread is working
	* Decrement the count
	*/
	void signalWorking()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		count--;
		_c.notify_all();
	}

	/*
	* Signal manager that the thread is done
	* Increment the count
	*/
	void signalDone()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		count++;
		_c.notify_all();
	}

	/*
	* Check if all the threads are busy or not
	* For Synchronization
	*/
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