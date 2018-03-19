#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "GameObject.h"
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
	RENDER_UPDATE,
	SWAP_COLOR,

	INPUT_READ_PRESSED = 0x200,// Input Section
	INPUT_READ_CONTINUOUS,

	ENGINE_MOVE_CAMERA_UP = 0x300,	// Engine Section
	ENGINE_MOVE_CAMERA_DOWN,
	ENGINE_MOVE_CAMERA_RIGHT,
	ENGINE_MOVE_CAMERA_LEFT,

	FILE_LOAD_TXT_DATA = 0x400,
	FILE_OBJ_LOAD,

	APPLICATION_ADD_OBJECTS = 0x500,
	APPLICATION_ADD_SINGLE_OBJECT
};

class System
{
public:
	System();
	~System();

	/*
	* Virtual Update funciton 
	* Threads will use the seperate system's update instead
	*/
	virtual void Update(JOB_TYPES t, BaseContent * ptr = nullptr) = 0;

	/*
	* Virtual Close function
	* Threads will use the seperate system's close instead
	*/
	virtual void Close() = 0;

protected:
	std::mutex _lockMutex;		// Mutex for when multiple threads are in the same system
	std::condition_variable _c;
};
