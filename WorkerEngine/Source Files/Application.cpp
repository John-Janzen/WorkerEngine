#include "Application.h"

Application::Application() {}

Application::~Application() { Close(); }

void Application::Init(uint16_t num)
{
	Manager::instance().Init(num);
	_scheduler = new Scheduler();

	addSystem("Engine", new Engine(this));
	renderCopy = new Render(this);
	addSystem("Render", renderCopy);
	addSystem("Input", new Input(this));
	addSystem("FileLoader", new FileLoader(this));
}

void Application::Close()
{
	for (GameObject * go : _worldObjects)
		delete(go);

	_worldObjects.clear();

	for (std::map<std::string, System*>::iterator i = _systems.begin(); i != _systems.end(); ++i)
		delete(i->second);

	_systems.clear();
	delete(_scheduler);
}

void Application::addSingleObject(GameObject * go)
{
	if (go != nullptr)
	{
		_worldObjects.emplace_back(go);
		printf("Loaded: %s\n", go->getName().c_str());
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
void Application::addJob(std::string name, JOB_TYPES j, BaseContent * ptr)
{
	addJob(std::make_shared<Job>(_systems[name], j, ptr));
}

/*
* Add a job to the list
*/
void Application::addJob(std::shared_ptr<Job> j)
{
	_scheduler->EmplaceJob(j);
}

void Application::initNumberObjects(int num)
{
	numOfObjects = num;
}
