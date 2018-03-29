#pragma once
#include <stdio.h>

#include "System.h"

class Engine : public System
{
public:
	Engine(Scheduler *_scheduler);
	~Engine();

	/*
	* The Update function that should be called
	* Currently does nothing
	*/
	virtual void Update(JOB_TYPES t, bool & flag, BaseContent* ptr = nullptr);

	void HandlePlayer(BaseContent * ptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

private:
	Scheduler * _scheduler;
};

