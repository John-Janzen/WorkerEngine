#pragma once

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
	int Get_ID() { return _id; }

	// Pointer to Job the thread needs to do
	std::shared_ptr<Job> a;	

private:
	std::thread * _thread;
	int _id;
	bool _dead;

};
