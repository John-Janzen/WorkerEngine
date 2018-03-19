#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(int n)
{
	Application::Init(n);
	Manager::instance().addJob("FileLoader", JOB_TYPES::FILE_LOAD_TXT_DATA, new FileToLoadContent("Assets/prototype.dat"));
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
	renderCopy->Update(JOB_TYPES::RENDER_UPDATE, new RenderUpdateContent(_worldObjects));		// Render the screen
	
	Uint32 frameTicks = SDL_GetTicks();
	if (frameTicks - now < SCREEN_TICKS_PER_FRAME)		// Wait time for synchronization
	{
		//printf("%d-", SCREEN_TICKS_PER_FRAME - (frameTicks - now));
		SDL_Delay(SCREEN_TICKS_PER_FRAME - (frameTicks - now));
	}
		
}
