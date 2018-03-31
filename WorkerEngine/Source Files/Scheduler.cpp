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
	std::shared_ptr<Job> j;
	while (Manager::instance().checkThreads() || !_jobs.empty())
	{
		if (!_jobs.empty())
		{
			std::unique_lock<std::mutex> lock(_lockMutex);
			Manager::instance().trasferList(_jobs);
			_jobs.clear();
			_c.notify_all();
		}
		if ((j = Manager::instance().AllocateJobs()) != nullptr)
		{
			bool flag = false;
			j->Get_System()->Update(j->Get_JobType(), flag, j->Get_Data());
			j = nullptr;
		}
	}
}

void Scheduler::EmplaceJob(std::shared_ptr<Job> job)
{
	_jobs.emplace_back(job);
}
