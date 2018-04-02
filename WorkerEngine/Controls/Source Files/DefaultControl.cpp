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
		printf("0 Pressed");
		break;
	case SDL_SCANCODE_9:
		input.ChangeState(&Control::_controlScheme2);
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