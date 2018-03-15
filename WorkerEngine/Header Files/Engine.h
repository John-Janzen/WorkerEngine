#pragma once
#include <stdio.h>

#include "System.h"

class Engine : public System
{
public:
	Engine();
	~Engine();

	/*
	* The Update function that should be called
	* Currently does nothing
	*/
	virtual void Update(JOB_TYPES t, void* ptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

private:
	
};

