#include "Input.h"

Input::Input(Application * a) : System(a)
{ 
	keys = SDL_GetKeyboardState(NULL); 
	_MoveCommand = new MoveCommand();
	currentControl = new DefaultControl("Initialized Default");
}

Input::~Input() { Close(); }

void Input::Update(JOB_TYPES job, bool & flag, BaseContent* ptr = nullptr)
{
	//Manager::instance().signalWorking();
	switch (job)
	{
	case INPUT_READ_PRESSED:
		ReadPress(ptr);
		break;
	case INPUT_READ_CONTINUOUS:
		readContinuous();
		break;
	case INPUT_ADD_PLAYER:
	{
		InputIPContent * IIPContent = static_cast<InputIPContent*>(ptr);
		_player = static_cast<Player*>(IIPContent->player);
		break;
	}
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
	//Manager::instance().signalDone();
}

void Input::Close()
{
	delete(_MoveCommand);
}

void Input::ReadPress(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	InputContent * IContent = static_cast<InputContent*>(ptr);
	SDL_Event e = IContent->Event;
	currentControl->ApplyControl(*this, e);
	_c.notify_one();
}

void Input::readContinuous()
{
	keys = SDL_GetKeyboardState(NULL);
	float moveX = 0, moveY = 0;
	if (keys[SDL_SCANCODE_A]) moveX += -10.0f;
	if (keys[SDL_SCANCODE_W]) moveY += 10.0f;
	if (keys[SDL_SCANCODE_S]) moveY += -10.0f;
	if (keys[SDL_SCANCODE_D]) moveX += 10.0f;

	if (moveX != 0 || moveY != 0)
	{
		_MoveCommand->x = moveX;
		_MoveCommand->y = moveY;
		if (_player != nullptr)
			_MoveCommand->execute(*_player);
	}
}

void Input::ChangeState(Control * c)
{
	currentControl = c;
}
