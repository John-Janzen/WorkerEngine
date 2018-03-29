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

	void addSystem(std::string key, System * s);

	void addJob(std::string name, JOB_TYPES j = SYSTEM_DEFAULT, BaseContent * ptr = nullptr);

	void addJob(std::shared_ptr<Job> j);


private:
	std::deque<std::shared_ptr<Job>> _jobs;
	std::map<std::string, System*> _systems;
	std::atomic<size_t> _jobsSize = 0;
	std::mutex _lockMutex;
	std::condition_variable _c;
};

