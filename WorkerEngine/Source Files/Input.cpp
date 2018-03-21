#include "Input.h"

Input::Input(GameObject * p) : _camera{ p } { keys = SDL_GetKeyboardState(NULL); }

Input::~Input() {}

void Input::Update(JOB_TYPES job, BaseContent* ptr = nullptr)
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
	if (ptr != nullptr)
		ptr = nullptr;
	Manager::instance().signalDone();
}

void Input::Close()
{
	
}

void Input::ReadPress(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	InputContent * IContent = static_cast<InputContent*>(ptr);
	SDL_Event * e = IContent->Event;
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
	_c.notify_one();
}

void Input::readContinuous()
{
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A]) _camera->adjustPosX(0.1f);
	if (keys[SDL_SCANCODE_W]) _camera->adjustPosY(-0.1f);
	if (keys[SDL_SCANCODE_S]) _camera->adjustPosY(0.1f);
	if (keys[SDL_SCANCODE_D]) _camera->adjustPosX(-0.1f);
	if (keys[SDL_SCANCODE_E]) _camera->adjustPosZ(0.1f);
	if (keys[SDL_SCANCODE_Q]) _camera->adjustPosZ(-0.1f);
}