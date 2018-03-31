#pragma once
#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "Manager.h"

class Scheduler
{
public:
	Scheduler();
	~Scheduler();

	void Close();

	void RunSchedule();

	void EmplaceJob(std::shared_ptr<Job> job);

private:
	std::deque<std::shared_ptr<Job>> _jobs;
	std::atomic<size_t> _jobsSize = 0;
	std::mutex _lockMutex;
	std::condition_variable _c;
};

