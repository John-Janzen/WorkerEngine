#include "Engine.h"

Engine::Engine(Application * a) : System(a) {};

Engine::~Engine(){ Close(); }

void Engine::Close() {}

void Engine::Update(JOB_TYPES t, bool & flag, BaseContent* ptr)
{
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
}

void Engine::HandlePlayer(BaseContent* ptr)
{
	EngineObjectContent * EOContent = static_cast<EngineObjectContent*> (ptr);
	EOContent->obj->Update(0, 0);
}
