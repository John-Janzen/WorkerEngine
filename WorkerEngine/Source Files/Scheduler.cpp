#include "Scheduler.h"
#include "System.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

void Scheduler::Close()
{
	for (std::map<std::string, System*>::iterator i = _systems.begin(); i != _systems.end(); ++i)
		i->second->Close();
	_systems.clear();
}

void Scheduler::RunSchedule()
{
	while (Manager::instance().checkThreads() || !_jobs.empty())
	{
		if (!_jobs.empty())
		{
			std::unique_lock<std::mutex> lock(_lockMutex);
			Manager::instance().trasferList(_jobs);
			_jobs.clear();
			_c.notify_all();
		}
		Manager::instance().AllocateJobs();
	}
}


/*
* Add a system to the map
*/
void Scheduler::addSystem(std::string key, System * s)
{
	_systems.emplace(std::make_pair(key, s));
}

/*
* Add a job to the list by:
* Name of system
* Job type
* Any void pointer
*/
void Scheduler::addJob(std::string name, JOB_TYPES j, BaseContent * ptr)
{
	addJob(std::make_shared<Job>(_systems[name], j, ptr));
}

/*
* Add a job to the list
*/
void Scheduler::addJob(std::shared_ptr<Job> j)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	_jobs.emplace_back(j);
	_c.notify_all();
}


