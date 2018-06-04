#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() 
{
	this->Close();
}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	loadBegin = SDL_GetTicks();
	printf("Time Load Start: %ums\n", loadBegin);
	currentScene = new MainMenuScene(this);
	_systems["Render"]->Update(RENDER_INIT, _flag);
}

bool MyApp::Update()
{
	now = SDL_GetTicks();

	JTime::instance().CalcDeltaTime();

	_scheduler->RunSchedule();

	while (!Manager::instance().checkDone())
	{
		if ((localJob = Manager::instance().AllocateJobs()) != nullptr)
		{
			bool flag = false;
			localJob->Get_System()->Update(localJob->Get_JobType(), flag, localJob->Get_Data());
			localJob = nullptr;
		}
	}

	switch (state)
	{
	case LOADING:
		if (currentScene->LoadScene(_cameraObject))
		{
			loadEnd = SDL_GetTicks();
			printf("Time Load End: %ums\n", loadEnd - loadBegin);
			_systems["Render"]->Update(RENDER_LOAD, _flag, new RenderLoadContent(currentScene->getSceneObjects(), _cameraObject));
			currentS = nextS;
			state = UPDATE;
			static_cast<Render*>(_systems["Render"])->DoneLoading();
		}
		
		break;
	case UPDATE:
	{
		currentScene->UpdateScene();

		_systems["Render"]->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(currentScene->getSceneObjects(), _cameraObject));

		frameTicks = SDL_GetTicks();
		//printf("%u-", frameTicks - now);
		if (nextS != currentS) state = UNLOAD;
		break;
	}
	case UNLOAD:
	{
		delete(currentScene);
		switch (nextS)
		{
		case MENU_SCENE:
			currentScene = new MainMenuScene(this);
			break;
		case PROTOTYPE_SCENE:
			currentScene = new PrototypeScene(this);
			printf("Loading Scene: Prototype\n");
			break;
		default:
			break;
		}
		state = LOADING;
		loadBegin = SDL_GetTicks();
		//printf("Time Load Start: %ums\n", loadBegin);
		static_cast<Render*>(_systems["Render"])->LoadingView();
		_cameraObject = nullptr;
		break;
	}
	default:
		break;
	}
	return quit;
}

void MyApp::setNextScene(const SCENE & s)
{
	nextS = s;
}
