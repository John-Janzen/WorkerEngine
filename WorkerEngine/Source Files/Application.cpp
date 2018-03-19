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

	Manager::instance().addSystem("Engine", new Engine());
	Manager::instance().addSystem("Render", renderCopy);
	Manager::instance().addSystem("Input", new Input(p));
	Manager::instance().addSystem("FileLoader", new FileLoader());
	Manager::instance().addSystem("Application", this);

	_worldObjects.emplace_back(p);
}

void Application::Update(JOB_TYPES t, BaseContent * ptr)
{
	Manager::instance().signalWorking();
	switch (t)
	{
	case SYSTEM_DEFAULT:
		break;
	case APPLICATION_ADD_OBJECTS:
		addWorldObject(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		ptr = nullptr;
	Manager::instance().signalDone();
}

void Application::Close()
{
	for (ThreadWorker * thread : _workers)
		delete(thread);

	for (GameObject * go : _worldObjects)
		delete(go);

	_worldObjects.clear();
}

void Application::addWorldObject(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	FileLoadedContent * FLContent = static_cast<FileLoadedContent*>(ptr);

	for (GameObject * go : FLContent->_objects)
		_worldObjects.emplace_back(go);

	_c.notify_one();
}
