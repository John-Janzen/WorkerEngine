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

	void Init(int i);
	void Close();

protected:
	std::vector<ThreadWorker*> _workers;
	Render * renderCopy;
};
