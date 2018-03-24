#include "Input.h"

Input::Input() { keys = SDL_GetKeyboardState(NULL); }

Input::~Input() {}

void Input::Update(JOB_TYPES job, bool & flag, BaseContent* ptr = nullptr)
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
		delete(ptr);
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
	float moveX = 0, moveY = 0, moveZ = 0;
	if (keys[SDL_SCANCODE_A]) moveX += 1.0f;
	if (keys[SDL_SCANCODE_W]) moveY += -1.0f;
	if (keys[SDL_SCANCODE_S]) moveY += 1.0f;
	if (keys[SDL_SCANCODE_D]) moveX += -1.0f;
	if (keys[SDL_SCANCODE_E]) moveZ += 1.0f;
	if (keys[SDL_SCANCODE_Q]) moveZ += -1.0f;

	if (moveX != 0 || moveY != 0 || moveZ != 0)
		Manager::instance().addJob("Render", JOB_TYPES::RENDER_HANDLE_CAMERA, new RenderCameraContent(moveX, moveY, moveZ));
}