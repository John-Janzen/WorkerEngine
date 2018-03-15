#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(int n)
{
	Application::Init(n);
	renderCopy->Update(JOB_TYPES::RENDER_LOAD);
}

void MyApp::Update(SDL_Event * e)
{
	Uint32 now = SDL_GetTicks();

	if (e->type != 0)
		Manager::instance().addJob("Input", JOB_TYPES::INPUT_READ, (void*)e);
	else
		delete(e);

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
	while (Manager::instance().checkBusy());
	renderCopy->Update(JOB_TYPES::RENDER_UPDATE);
	
	int frameTicks = SDL_GetTicks();
	if (frameTicks - now < SCREEN_TICKS_PER_FRAME)
		SDL_Delay(frameTicks - now);
}
