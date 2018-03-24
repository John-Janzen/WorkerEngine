#pragma once

#include "System.h"

class Job
{
public:
	/*
	* A Job that points to the system where the thread needs to go
	* The job type the thread needs to do on the system
	* Any type of data that needs to be in pointer form
	*/
	Job(System * sys, JOB_TYPES type, BaseContent * ptr = nullptr) : _system{ sys }, _type{ type }, _ptr{ ptr } {};

	~Job() 
	{
		_system = nullptr;
		if (_ptr != nullptr)
			_ptr = nullptr;
		_type = JOB_TYPES::SYSTEM_DEFAULT;
	};
	
	/*
	* Get the current system that the thread needs to go to
	*/
	System* Get_System() { return _system; }

	/*
	* Get the job type the thread needs to do
	*/
	JOB_TYPES Get_JobType() { return _type; }

	/*
	* Get the data that is passed through as a pointer
	* static_cast<?>(_ptr) **REQUIRED**
	*/
	BaseContent * Get_Data() { return _ptr; }

private:
	System * _system;
	JOB_TYPES _type;
	BaseContent * _ptr;
};