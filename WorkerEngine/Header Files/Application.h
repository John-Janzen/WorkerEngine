#pragma once

#include "Render.h"
#include "Engine.h"
#include "Input.h"
#include "FileLoader.h"

class Application 
{
public:
	Application();
	~Application();

	/*
	* Initializes Application systems and threads (Workers)
	* Also Initializes Manager.
	*/
	void Init(uint16_t i);

	/*
	* Closes this application
	* Pointers should be deleted
	*/
	void Close();

	void addSingleObject(GameObject * go);

	void addSystem(std::string key, System * s);

	void addJob(std::string name, JOB_TYPES j = SYSTEM_DEFAULT, BaseContent * ptr = nullptr);

	void addJob(std::shared_ptr<Job> j);

	void initNumberObjects(int num);

protected:
	Render * renderCopy;
	std::vector<GameObject*> _worldObjects;
	std::map<std::string, System*> _systems;
	Scheduler * _scheduler;
	std::mutex _lockMutex;
	std::condition_variable _c;
	int numOfObjects = -1;
};
