#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

enum JOB_TYPES
{
	SYSTEM_DEFAULT = 0x000,
	RENDER_LOAD,
	RENDER_UPDATE,
	SWAP_COLOR,
	INPUT_READ
};

class System
{
public:
	System();
	~System();

	virtual void Update(JOB_TYPES t, void* ptr = nullptr) = 0;
	virtual void Close() = 0;

protected:
	std::mutex _lockMutex;
	std::condition_variable _c;
};
