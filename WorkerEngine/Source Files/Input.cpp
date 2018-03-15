#include "Input.h"

Input::Input() {}

Input::~Input() {}

void Input::Update(JOB_TYPES job, void* ptr = nullptr)
{
	Manager::instance().signalWorking();
	switch (job)
	{
	case INPUT_READ:
		Read(ptr);
		break;
	default:
		break;
	}
	Manager::instance().signalDone();
}

void Input::Close()
{

}

void Input::Read(void* ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	if (ptr != nullptr)
	{
		SDL_Event * e = static_cast<SDL_Event*>(ptr);
		switch (e->key.keysym.sym)
		{
		case SDLK_0:
			printf("0 pressed\n");
			break;
		case SDLK_q:
			Manager::instance().addJob("Render", JOB_TYPES::SWAP_COLOR);
			break;
		default:
			break;
		}
		delete(e);
	}
	_c.notify_one();
}
