#pragma once

#include "Control.h"
class DefaultControl : public Control
{
public:
	DefaultControl(const std::string & n);
	~DefaultControl();

	void ApplyControl(Input & input, const SDL_Event & e);
protected:
	SDL_bool _mouseLocked = SDL_TRUE;
};

