#include "Engine.h"

Engine::Engine() {};

Engine::~Engine(){}

void Engine::Close()
{

}

void Engine::Update(JOB_TYPES t, void* ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	switch (t)
	{
	case SYSTEM_DEFAULT:
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		ptr = nullptr;
	_c.notify_one();
}
