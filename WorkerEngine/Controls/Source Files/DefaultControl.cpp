#include "DefaultControl.h"
#include "Input.h"

DefaultControl::DefaultControl(const std::string & n) : Control(n) {}

DefaultControl::~DefaultControl() {}

void DefaultControl::ApplyControl(Input & input, const SDL_Event & e)
{
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		printf("Escape Pressed\n");
		break;
	case SDL_SCANCODE_0:
		printf("0 Pressed\n");
		break;
	case SDL_SCANCODE_9:
		input.ChangeState(&input._controlScheme2);
		printf("Switched to %s\n", input._controlScheme2.getName().c_str());
		break;
	case SDL_SCANCODE_K:
		if (_mouseLocked == SDL_TRUE)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			_mouseLocked = SDL_FALSE;
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			_mouseLocked = SDL_TRUE;
		}
		break;
	default:
		break;
	}
}

const std::string & DefaultControl::getName()
{
	return _name;
}
