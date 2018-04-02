#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() 
{
	//_scheduler->Close();
}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	currentScene = new MainMenuScene(this);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	_systems["Render"]->Update(RENDER_INIT, _flag);
	currentScene->InitScene();
}

bool MyApp::Update()
{
	now = SDL_GetTicks();

	JTime::instance().CalcDeltaTime();

	_scheduler->RunSchedule();

	switch (state)
	{
	case LOADING:
		if (currentScene->LoadScene(_cameraObject))
		{
			printf("Time Load End: %ums\n", SDL_GetTicks());
			_systems["Render"]->Update(RENDER_LOAD, _flag, new RenderLoadContent(currentScene->getSceneObjects(), _cameraObject));
			state = UPDATE;
		}
		
		break;
	case UPDATE:
	{
		currentScene->UpdateScene();

		while (Manager::instance().checkDone());			// Wait for the threads to finish

		_systems["Render"]->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(currentScene->getSceneObjects(), _cameraObject));

		frameTicks = SDL_GetTicks();
		//printf("%u-", frameTicks - now);
		break;
	}
	default:
		break;
	}
	return quit;
}