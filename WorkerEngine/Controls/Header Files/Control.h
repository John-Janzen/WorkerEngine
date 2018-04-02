#pragma once
#include <string>
#include <SDL.h>

class Input;
class Control
{
public:
	Control(const std::string & n) : _name(n) {}
	virtual ~Control() {}

	virtual void ApplyControl(Input & input, const SDL_Event & e) = 0;

	static DefaultControl _controlScheme1;
	static MenuControl _controlScheme2;
	
protected:
	std::string _name;
};
