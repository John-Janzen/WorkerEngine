#pragma once
#include "Command.h"
class MoveCommand : public Command
{
public:
	MoveCommand() {}
	~MoveCommand() {}
	void execute(Player & player)
	{
		player.Update(x, y);
	}
	float x = 0, y = 0;
};
