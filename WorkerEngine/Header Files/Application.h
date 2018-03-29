#pragma once

#include "Render.h"
#include "Engine.h"
#include "Input.h"
#include "FileLoader.h"

class Application : public System
{
public:
	Application();
	~Application();

	/*
	* Initializes Application systems and threads (Workers)
	* Also Initializes Manager.
	*/
	void Init(uint16_t i);

	virtual void Update(JOB_TYPES t, bool & flag, BaseContent * ptr = nullptr);

	/*
	* Closes this application
	* Pointers should be deleted
	*/
	void Close();

	void addSingleObject(BaseContent * ptr);

protected:
	Render * renderCopy;
	std::vector<GameObject*> _worldObjects;
	Scheduler * _scheduler;
	int numOfObjects = -1;
};
