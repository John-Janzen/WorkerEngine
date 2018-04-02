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
protected:
	std::string _name;
};
