#include "Engine.h"
#include "Application.h"

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
	case CHANGE_SCENE:
		ChangeScene(ptr);
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

void Engine::ChangeScene(BaseContent * ptr)
{
	ChangeSceneContent * CSContent = static_cast<ChangeSceneContent*>(ptr);
	_app->setNextScene(CSContent->sceneName);
}
