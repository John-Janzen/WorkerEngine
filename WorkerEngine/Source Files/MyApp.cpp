#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(int n)
{
	Application::Init(n);
	renderCopy->Update(JOB_TYPES::RENDER_LOAD);
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
	while (Manager::instance().checkBusy());			// Wait for the threads to finish

	renderCopy->Update(JOB_TYPES::RENDER_HANDLE_CAMERA, (void*)_worldObjects["Camera"]);
	renderCopy->Update(JOB_TYPES::RENDER_UPDATE);		// Render the screen
	
	int frameTicks = SDL_GetTicks();
	if (frameTicks - now < SCREEN_TICKS_PER_FRAME)		// Wait time for synchronization
		SDL_Delay(SCREEN_TICKS_PER_FRAME - (frameTicks - now));
}
