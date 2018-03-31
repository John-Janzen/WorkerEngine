#pragma once
#include <SDL.h>

#include "System.h"
#include "Scheduler.h"
#include "MoveCommand.h"

class Input : public System
{
public:
	Input(Application * a);
	~Input();

	/*
	* Update function that checks the job types to see what function to use
	*/
	virtual void Update(JOB_TYPES j, bool & flag, BaseContent * ptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

	void ReadPress(BaseContent* ptr);

	void readContinuous();

private:
	const Uint8 * keys;	
	GameObject * _camera;
	Player * _player;
	MoveCommand * _MoveCommand;
};

