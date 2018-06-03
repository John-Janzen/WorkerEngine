#include "Scheduler.h"
#include "System.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() 
{
	Close();
}

void Scheduler::Close() {}

void Scheduler::RunSchedule()
{
	if (!_jobs.empty())
	{
		Manager::instance().trasferList(_jobs);
		_jobs.clear();
	}
}

void Scheduler::EmplaceJob(std::shared_ptr<Job> job)
{
	_jobs.emplace_back(job);
}
