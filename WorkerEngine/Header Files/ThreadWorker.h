#pragma once

#include <SDL.h>
#include <string>
#include <thread>
#include <time.h>
#include <chrono>

#include "Job.h"

class ThreadWorker
{
public:
	ThreadWorker(int i);
	~ThreadWorker();
	
	void Running();

	std::thread * Get_Worker() { return _thread; }
	void Set_Worker(std::thread * t) { _thread = t; }

	int Get_ID() { return _id; }

	std::shared_ptr<Job> a;

private:
	std::thread * _thread;
	int _id;
	bool _dead;

};
