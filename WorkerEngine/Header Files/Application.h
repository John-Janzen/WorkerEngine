#pragma once

#include "ThreadWorker.h"
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
	void Init(int i);

	virtual void Update(JOB_TYPES t, BaseContent * ptr = nullptr);

	/*
	* Closes this application
	* Pointers should be deleted
	*/
	void Close();

	void addWorldObject(BaseContent *);

protected:
	std::vector<ThreadWorker*> _workers;
	Render * renderCopy;
	std::vector<GameObject*> _worldObjects;
};
