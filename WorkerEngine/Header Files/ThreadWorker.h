#pragma once

#include <string>
#include <thread>
#include <time.h>
#include <chrono>

#include "Job.h"
class ThreadPool;
class ThreadWorker
{
public:
	ThreadWorker(const size_t & i, ThreadPool * tp);
	~ThreadWorker();
	
	/*
	* Continuous loop for the thread
	*/
	void Running();

	/*
	* Gets the std::thread
	*/
	std::thread * Get_Worker() { return _thread; }

	/*
	* Sets the std::thread
	*/
	void Set_Worker(std::thread * t) { _thread = t; }

	/*
	* Get the ID of the thread
	*/
	char* Get_Name() { return _name; }

	// Pointer to Job the thread needs to do
	std::shared_ptr<Job> a;	

private:
	std::thread * _thread;
	char _name[30];
	bool _dead, _flag;
	ThreadPool * _parent;
};
