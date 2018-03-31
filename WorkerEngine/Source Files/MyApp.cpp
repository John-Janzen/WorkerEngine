#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() 
{
	//_scheduler->Close();
}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	renderCopy->Update(RENDER_INIT, _flag);
	addJob("FileLoader", FILE_LOAD_EXTERNAL, new FileToLoadContent("Assets/prototype.dat"));
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
		if (_worldObjects.size() == numOfObjects && !Manager::instance().checkDone())
		{
			printf("Loading started");
			for (GameObject * obj : _worldObjects)
			{
				if (obj->getName().compare("Camera") == 0)
				{
					renderCopy->Update(RENDER_LOAD, _flag, new RenderLoadContent(&_worldObjects, obj));
					break;
				}
			}
			printf("Time Load End: %ums\n", SDL_GetTicks());
			state = UPDATE;
		}
		break;
	case UPDATE:
	{
		success = ReadInputs();
		
		for (GameObject * go : _worldObjects)
			addJob("Engine", ENGINE_HANDLE_OBJECT, new EngineObjectContent(go));

		while (Manager::instance().checkDone());			// Wait for the threads to finish

		renderCopy->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(&_worldObjects));		// Render the screen
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
				addJob("Input", JOB_TYPES::INPUT_READ_PRESSED, new InputContent(&e));	// Send Job if Event is changed
			break;
		default:
			break;
		}
	}
	addJob("Input", JOB_TYPES::INPUT_READ_CONTINUOUS);	// Read held keys
	return false;
}
