#pragma once
#include "Control.h"
class MenuControl : public Control
{
public:
	MenuControl(const std::string & s);
	~MenuControl();

	void ApplyControl(Input & input, const SDL_Event & e);
private:

};

