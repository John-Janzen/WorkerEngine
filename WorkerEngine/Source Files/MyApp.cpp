#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(int n)
{
	Application::Init(n);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	Manager::instance().addJob("FileLoader", FILE_LOAD_TXT_DATA, new FileToLoadContent("Assets/prototype.dat"));
}

void MyApp::Update()
{
	Uint32 now = SDL_GetTicks();
	while (Manager::instance().hasJobs() && !Manager::instance().checkBusy())
	{
		for (ThreadWorker * t : _workers)
		{
			if (t->a == nullptr)
			{
				Manager::instance().Give_Job(t);
				break;
			}
		}
	}
	switch (state)
	{
	case LOADING:
		if (_worldObjects.size() == numOfObjects && !Manager::instance().hasJobs() && !Manager::instance().checkBusy())
		{
			for (GameObject * obj : _worldObjects)
			{
				if (obj->getName().compare("Camera") == 0)
				{
					renderCopy->Update(RENDER_LOAD, new RenderLoadContent(&_worldObjects, obj));
					break;
				}
			}
			printf("Time Load End: %ums\n", SDL_GetTicks());
			state = UPDATE;
		}
		break;
	case UPDATE:
	{
		while (Manager::instance().checkBusy());			// Wait for the threads to finish
		renderCopy->Update(RENDER_UPDATE, new RenderUpdateContent(&_worldObjects));		// Render the screen

		Uint32 frameTicks = SDL_GetTicks();
		if (frameTicks - now < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - (frameTicks - now));
		}
		

		break;
	}
	default:
		break;
	}
}
