#include "Application.h"

Application::Application() {}

Application::~Application() {}

void Application::Init(int num)
{
	for (int i = 0; i < num; i++)
		_workers.emplace_back(new ThreadWorker(i));

	Manager::instance().passCount(num);

	renderCopy = new Render();
	GameObject* p = new GameObject("Camera", 01);

	Manager::instance().addSystem("Engine", new Engine(p));
	Manager::instance().addSystem("Render", renderCopy);
	Manager::instance().addSystem("Input", new Input());
	Manager::instance().addSystem("FileLoader", new FileLoader());

	_worldObjects.emplace(std::make_pair("Camera", p));
}

void Application::Close()
{
	for (ThreadWorker * thread : _workers)
		delete(thread);

	for (std::map<std::string, GameObject*>::iterator it = _worldObjects.begin(); it != _worldObjects.end(); ++it)
		delete(it->second);

	_worldObjects.clear();
	
	Manager::instance().Close();
}