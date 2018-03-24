#include "Application.h"

Application::Application() {}

Application::~Application() {}

void Application::Init(uint16_t num)
{
	for (int i = 0; i < num; i++)
		_workers.emplace_back(new ThreadWorker(i));

	Manager::instance().passCount(num);

	renderCopy = new Render();

	Manager::instance().addSystem("Engine", new Engine());
	Manager::instance().addSystem("Render", renderCopy);
	Manager::instance().addSystem("Input", new Input());
	Manager::instance().addSystem("FileLoader", new FileLoader());
	Manager::instance().addSystem("Application", this);
}

void Application::Update(JOB_TYPES t, bool &flag, BaseContent * ptr)
{
	Manager::instance().signalWorking();
	switch (t)
	{
	case SYSTEM_DEFAULT:
		break;
	//case APPLICATION_ADD_OBJECTS:
	//	addWorldObject(ptr);
	//	break;
	case APPLICATION_ADD_SINGLE_OBJECT:
		addSingleObject(ptr);
		break;
	case APPLICATION_NUMBER_OBJECTS:
	{
		IntPassContent * IPContent = static_cast<IntPassContent*>(ptr);
		numOfObjects = IPContent->num;
		break;
	}
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
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

void Application::addSingleObject(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	FileLoadedContent * FLContent = static_cast<FileLoadedContent*>(ptr);

	if (FLContent->object != nullptr)
	{
		_worldObjects.emplace_back(FLContent->object);
		printf("Loaded: %s\n", FLContent->object->getName().c_str());
	}
	_c.notify_one();
}