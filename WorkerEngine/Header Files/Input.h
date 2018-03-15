#pragma once
#include <SDL.h>

#include "System.h"
#include "Manager.h"

class Input : public System
{
public:
	Input();
	~Input();

	virtual void Update(JOB_TYPES j, void* ptr);
	virtual void Close();

	void Read(void* ptr);

private:
};

