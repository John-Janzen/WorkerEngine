#pragma once
#include <SDL.h>

#include "System.h"
#include "Manager.h"

class Input : public System
{
public:
	Input();
	~Input();

	/*
	* Update function that checks the job types to see what function to use
	*/
	virtual void Update(JOB_TYPES j, void* ptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

	void Read(void* ptr);

private:
};

