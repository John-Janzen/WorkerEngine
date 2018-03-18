#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "GameObject.h"

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
	RENDER_HANDLE_CAMERA,

	INPUT_READ_PRESSED = 0x200,// Input Section
	INPUT_READ_CONTINUOUS,

	ENGINE_MOVE_CAMERA_UP = 0x300,	// Engine Section
	ENGINE_MOVE_CAMERA_DOWN,
	ENGINE_MOVE_CAMERA_RIGHT,
	ENGINE_MOVE_CAMERA_LEFT,

	FILE_LOAD_TXT_DATA = 0x400,

	APPLICATION_ADD_OBJECT = 0x500
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
	virtual void Update(JOB_TYPES t, void* ptr = nullptr) = 0;

	/*
	* Virtual Close function
	* Threads will use the seperate system's close instead
	*/
	virtual void Close() = 0;

protected:
	std::mutex _lockMutex;		// Mutex for when multiple threads are in the same system
	std::condition_variable _c;
};
