#pragma once

#include "Content.h"
/*
* When adding a new systemt to the manager queue
* said system will need a update and close function as per the virtual function.
*
* Furthermore, when the new system has the update function it will
* need to follow the following example:

Update(JOB_TYPE t, BaseContent* ptr) {
	Manager::instance().signalWorking();
	switch (t)
	{
	case SYSTEM_DEFAULT:
	break;
	default:
	break;
	}
	if (ptr != nullptr)
	ptr = nullptr;
	Manager::instance().signalDone();
}

* Close doesn't matter it is for deallocating data
* when the program closes.
*
* When functions are called that could be called by different threads
* It is required to unqiue lock the system's mutex and notify either on or all
* At the end
*/

/*
* The list of job types that each system can do
* Use the switch statement to organize the job types
* Any new system increase the 0x?00 number by 1
*/
enum JOB_TYPES
{
	SYSTEM_DEFAULT = 0x000, // Unused

	RENDER_LOAD = 0x100,	// Render Section
	RENDER_INIT,
	RENDER_UPDATE,
	RENDER_HANDLE_CAMERA,

	INPUT_READ_PRESSED = 0x200,// Input Section
	INPUT_READ_CONTINUOUS,
	INPUT_ADD_PLAYER,

	ENGINE_HANDLE_OBJECT = 0x300,	// Engine Section

	FILE_LOAD_TXT_DATA = 0x400,
	FILE_LOAD_GAMEOBJECT,
	FILE_LOAD_MODEL,
	FILE_LOAD_EXTERNAL,
	FILE_LOAD_TEXTURE,

	APPLICATION_ADD_OBJECTS = 0x500,
	APPLICATION_ADD_SINGLE_OBJECT,
	APPLICATION_NUMBER_OBJECTS,
	CHANGE_SCENE
};

class System;
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