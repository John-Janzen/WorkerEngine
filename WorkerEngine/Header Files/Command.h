#pragma once
#include "Player.h"
class Command
{
public:
	Command() {}
	virtual ~Command() {}
	virtual void execute(Player & player) = 0;
};

