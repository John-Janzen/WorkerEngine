#pragma once
#include <stdio.h>

#include "System.h"

class Engine : public System
{
public:
	Engine();
	~Engine();

	virtual void Update(JOB_TYPES t, void* ptr);
	virtual void Close();

private:
	
};

