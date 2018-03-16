#include "Engine.h"

Engine::Engine(GameObject * p) : _camera{ p } {};

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

	case ENGINE_MOVE_CAMERA_UP:
		_camera->adjustPosY(-16.f);
		break;
	case ENGINE_MOVE_CAMERA_DOWN:
		_camera->adjustPosY(16.f);
		break;
	case ENGINE_MOVE_CAMERA_RIGHT:
		_camera->adjustPosX(16.f);
		break;
	case ENGINE_MOVE_CAMERA_LEFT:
		_camera->adjustPosX(-16.f);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		ptr = nullptr;
	_c.notify_one();
}
