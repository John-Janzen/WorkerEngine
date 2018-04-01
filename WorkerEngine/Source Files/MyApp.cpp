#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() 
{
	//_scheduler->Close();
}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	currentScene = new MainMenuScene(this, renderCopy);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	renderCopy->Update(RENDER_INIT, _flag);
	currentScene->InitScene();
}

bool MyApp::Update()
{
	bool success = false;
	now = SDL_GetTicks();

	JTime::instance().CalcDeltaTime();

	_scheduler->RunSchedule();

	switch (state)
	{
	case LOADING:
		if (currentScene->LoadScene())
		{
			printf("Time Load End: %ums\n", SDL_GetTicks());
			state = UPDATE;
		}
		
		break;
	case UPDATE:
	{
		success = ReadInputs();
		
		currentScene->UpdateScene();

		frameTicks = SDL_GetTicks();
		//printf("%u-", frameTicks - now);
		break;
	}
	default:
		break;
	}
	return success;
}

bool MyApp::ReadInputs()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))// Listen for events
	{
		switch (e.type)
		{
		case SDL_QUIT:		// Quit the game
			return true;
			break;
		case SDL_KEYDOWN:
			if (e.key.repeat == 0)
				addJob("Input", JOB_TYPES::INPUT_READ_PRESSED, new InputContent(e));	// Send Job if Event is changed
			break;
		default:
			break;
		}
	}
	addJob("Input", JOB_TYPES::INPUT_READ_CONTINUOUS);	// Read held keys
	return false;
}
