#include "Application.h"

Application::Application() {}

Application::~Application() {}

void Application::Init(uint16_t num)
{
	Manager::instance().Init(num);
	_scheduler = new Scheduler();

	addSystem("Engine", new Engine(this));
	addSystem("Render", new Render(this));
	addSystem("Input", new Input(this));
	addSystem("FileLoader", new FileLoader(this));
}

void Application::Close()
{
	for (std::map<std::string, System*>::iterator i = _systems.begin(); i != _systems.end(); ++i)
		delete(i->second);

	_systems.clear();
	delete(_scheduler);
}

void Application::Quit()
{
	quit = true;
}

void Application::addSingleObject(GameObject * go)
{
	if (go != nullptr)
	{
		std::unique_lock<std::mutex> lock(_lockMutex);
		currentScene->AddObject(go);
		printf("Loaded: %s\n", go->getName().c_str());
		_c.notify_all();
	}
}

/*
* Add a system to the map
*/
void Application::addSystem(std::string key, System * s)
{
	_systems.emplace(std::make_pair(key, s));
}

/*
* Add a job to the list by:
* Name of system
* Job type
* Any void pointer
*/
void Application::addJob(std::string name, JOB_TYPES j, WHICH_THREAD which, BaseContent * ptr)
{
	addJob(std::make_shared<Job>(_systems[name], j, which, ptr));
}

/*
* Add a job to the list
*/
void Application::addJob(std::shared_ptr<Job> j)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	_scheduler->EmplaceJob(j);
	_c.notify_all();
}

void Application::initNumberObjects(int num)
{
	currentScene->initNumberObjects(num);
}
