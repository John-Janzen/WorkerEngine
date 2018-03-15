#pragma once

#include <vector>
#include <map>
#include <iostream>

#include "ThreadWorker.h"
#include "Engine.h"
#include "Render.h"
#include "Input.h"

class Application
{
public:
	Application();
	~Application();

	/*
	* Initializes Application systems and threads (Workers)
	* Also Initializes Manager.
	*/
	void Init(int i);

	/*
	* Closes this application
	* Pointers should be deleted
	*/
	void Close();

protected:
	std::vector<ThreadWorker*> _workers;
	Render * renderCopy;
};
