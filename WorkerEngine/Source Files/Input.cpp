#include "Input.h"

Input::Input() { keys = SDL_GetKeyboardState(NULL); }

Input::~Input() {}

void Input::Update(JOB_TYPES job, void* ptr = nullptr)
{
	Manager::instance().signalWorking();
	switch (job)
	{
	case INPUT_READ_PRESSED:
		ReadPress(ptr);
		break;
	default:
		break;
	}
	Manager::instance().signalDone();
}

void Input::Close()
{
	
}

void Input::ReadPress(void* ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	if (ptr != nullptr)
	{
		SDL_Event * e = static_cast<SDL_Event*>(ptr);
		switch (e->key.state)
		{
		case SDLK_0:
			printf("0 Pressed");
			break;
		case SDLK_q:
			Manager::instance().addJob("Render", JOB_TYPES::SWAP_COLOR);
			break;
		case SDLK_w:
			Manager::instance().addJob("Engine", JOB_TYPES::ENGINE_MOVE_CAMERA_UP);
			break;
		case SDLK_a:
			Manager::instance().addJob("Engine", JOB_TYPES::ENGINE_MOVE_CAMERA_LEFT);
			break;
		case SDLK_s:
			Manager::instance().addJob("Engine", JOB_TYPES::ENGINE_MOVE_CAMERA_DOWN);
			break;
		case SDLK_d:
			Manager::instance().addJob("Engine", JOB_TYPES::ENGINE_MOVE_CAMERA_RIGHT);
			break;
		default:
			break;
		}
		switch (e->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			printf("Escape Pressed\n");
			break;
		default: 
			break;
		}
		ptr = nullptr;
	}
	_c.notify_one();
}
