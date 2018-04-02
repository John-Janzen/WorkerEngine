#include "MenuControl.h"
#include "Input.h"

MenuControl::MenuControl(const std::string & s) : Control(s) {}

MenuControl::~MenuControl() {}

void MenuControl::ApplyControl(Input & input, const SDL_Event & e)
{
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		printf("Escape 2 Pressed\n");
		break;
	case SDL_SCANCODE_1:
		printf("1 Pressed\n");
		break;
	case SDL_SCANCODE_9:
		input.ChangeState(&input._controlScheme1);
		printf("Switched to %s\n", input._controlScheme1.getName().c_str());
		break;
	case SDL_SCANCODE_P:
		input.ChangeScene(PROTOTYPE_SCENE);
		break;
	default:
		break;
	}
}

const std::string & MenuControl::getName()
{
	return _name;
}
