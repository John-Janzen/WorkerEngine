#pragma once

#include "Render.h"
#include "Engine.h"
#include "Input.h"
#include "FileLoader.h"
#include "../Scenes/Header Files/MainMenuScene.h"
#include "../Scenes/Header Files/PrototypeScene.h"

class Application 
{
public:
	Application();
	~Application();


	/*
	* Initializes Application systems and threads (Workers)
	* Also Initializes Manager.
	*/
	void Init(const size_t & num);

	/*
	* Closes this application
	* Pointers should be deleted
	*/
	void Close();

	void Quit();

	void addSingleObject(GameObject * go);

	void addSystem(std::string key, System * s);

	void addJob(std::string name, JOB_TYPES j, WHICH_THREAD which, BaseContent * ptr = nullptr);

	void addJob(std::shared_ptr<Job> j);

	void initNumberObjects(int num);

	virtual void setNextScene(const SCENE & s) = 0;

protected:
	std::map<std::string, System*> _systems;
	Scheduler * _scheduler;
	std::mutex _lockMutex;
	std::condition_variable _c;
	int numOfObjects = -1;
	Scene * currentScene;
	GameObject * _cameraObject;
	bool quit = false;
	SCENE nextS = MENU_SCENE, currentS = MENU_SCENE;
};
