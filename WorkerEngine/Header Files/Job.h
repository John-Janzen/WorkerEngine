#pragma once

#include "System.h"

class Job
{
public:
	Job(System * sys, JOB_TYPES t, void* ptr = nullptr) : _system{ sys }, _type{ t }, _ptr{ ptr } {};
	~Job() 
	{
		_system = nullptr;
		_ptr = nullptr;
		_type = JOB_TYPES::SYSTEM_DEFAULT;
	};

	System* Get_System() { return _system; }
	JOB_TYPES Get_JobType() { return _type; }
	void* Get_Data() { return _ptr; }

private:
	System * _system;
	JOB_TYPES _type;
	void* _ptr;
};