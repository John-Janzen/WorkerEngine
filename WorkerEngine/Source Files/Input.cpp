#include "Input.h"

Input::Input(GameObject * p) : _camera{ p } { keys = SDL_GetKeyboardState(NULL); }

Input::~Input() {}

void Input::Update(JOB_TYPES job, void* ptr = nullptr)
{
	Manager::instance().signalWorking();
	switch (job)
	{
	case INPUT_READ_PRESSED:
		ReadPress(ptr);
		break;
	case INPUT_READ_CONTINUOUS:
		readContinuous();
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

void Input::readContinuous()
{
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A]) _camera->adjustPosX(16.f);
	if (keys[SDL_SCANCODE_W]) _camera->adjustPosY(16.f);
	if (keys[SDL_SCANCODE_S]) _camera->adjustPosY(-16.f);
	if (keys[SDL_SCANCODE_D]) _camera->adjustPosX(-16.f);
}