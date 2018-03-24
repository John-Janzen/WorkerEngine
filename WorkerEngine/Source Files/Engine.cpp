#include "Engine.h"

Engine::Engine() {};

Engine::~Engine(){}

void Engine::Close()
{

}

void Engine::Update(JOB_TYPES t, bool & flag, BaseContent* ptr)
{
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
