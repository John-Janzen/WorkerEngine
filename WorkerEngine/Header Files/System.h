#pragma once
#include <mutex>
#include <condition_variable>

#include "Quad.h"
#include "Player.h"
#include "Scheduler.h"
class Application;
class System
{
public:
	System(Application * a);
	~System();

	/*
	* Virtual Update funciton 
	* Threads will use the seperate system's update instead
	*/
	virtual void Update(JOB_TYPES t, bool & flag, BaseContent * ptr = nullptr) = 0;

	/*
	* Virtual Close function
	* Threads will use the seperate system's close instead
	*/
	virtual void Close() = 0;

protected:
	std::mutex _lockMutex;		// Mutex for when multiple threads are in the same system
	std::condition_variable _c;
	Application * _app;
};
