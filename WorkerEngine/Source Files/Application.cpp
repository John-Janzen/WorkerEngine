#include "Application.h"

Application::Application() {}

Application::~Application() {}

void Application::Init(int num)
{

	for (int i = 0; i < num; i++)
		_workers.emplace_back(new ThreadWorker(i));
	Manager::instance().passCount(num);

	renderCopy = new Render();

	Manager::instance().addSystem("Engine", new Engine());
	Manager::instance().addSystem("Render", renderCopy);
	Manager::instance().addSystem("Input", new Input());
}

void Application::Close()
{
	for (ThreadWorker * thread : _workers)
		delete(thread);
	
	Manager::instance().Close();
}