#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

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
	INPUT_READ = 0x200		// Input Section
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
