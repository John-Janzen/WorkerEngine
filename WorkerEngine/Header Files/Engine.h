#pragma once
#include <stdio.h>

#include "System.h"
#include "Manager.h"

class Engine : public System
{
public:
	Engine();
	~Engine();

	/*
	* The Update function that should be called
	* Currently does nothing
	*/
	virtual void Update(JOB_TYPES t, BaseContent* ptr = nullptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

private:
};

