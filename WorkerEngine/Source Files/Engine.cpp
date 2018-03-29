#include "Engine.h"

Engine::Engine(Scheduler * sch) : _scheduler(sch) {};

Engine::~Engine(){}

void Engine::Close()
{

}

void Engine::Update(JOB_TYPES t, bool & flag, BaseContent* ptr)
{
	//Manager::instance().signalWorking();
	switch (t)
	{
	case SYSTEM_DEFAULT:
		break;
	case ENGINE_HANDLE_OBJECT:
		HandlePlayer(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
	//Manager::instance().signalDone();
}

void Engine::HandlePlayer(BaseContent* ptr)
{
	EngineObjectContent * EOContent = static_cast<EngineObjectContent*> (ptr);
	EOContent->obj->Update(0, 0);
}
