#pragma once

#include "Job.h"

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
		_actions.emplace(j);
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
	std::shared_ptr<Job> Give_Job()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		if (hasJobs()){
			std::shared_ptr<Job> temp = _actions.front();
			_actions.pop();
			_c.notify_all();
			return temp;
		}
		else {
			_c.notify_all();
			return nullptr;
		}
	}

	/*
	* Check if the list has any jobs
	*/
	bool hasJobs()
	{
		return (_actions.empty()) ? false : true;
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

	bool checkFree()
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		bool check;
		if (count > 0)
			check = true;
		else
			check = false;
		_c.notify_all();
		return check;
	}

private:
	Manager() {}
	int count, max;
	std::queue<std::shared_ptr<Job>> _actions;
	std::map<std::string, System*> _systems;
	std::mutex _lockMutex;
	std::condition_variable _c;
};