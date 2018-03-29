#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	renderCopy->Update(RENDER_INIT, _flag);
	_scheduler->addJob("FileLoader", FILE_LOAD_TXT_DATA, new FileToLoadContent("Assets/prototype.dat"));
}

bool MyApp::Update()
{
	now = SDL_GetTicks();
	bool success = false;
	_scheduler->RunSchedule();

	switch (state)
	{
	case LOADING:
		if (_worldObjects.size() == numOfObjects)
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
			_scheduler->addJob("Engine", ENGINE_HANDLE_OBJECT, new EngineObjectContent(go, 50, 50));

		while (Manager::instance().checkDone());			// Wait for the threads to finish

		renderCopy->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(&_worldObjects));		// Render the screen
		frameTicks = SDL_GetTicks();
		printf("%u-", frameTicks - now);
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
				_scheduler->addJob("Input", JOB_TYPES::INPUT_READ_PRESSED, new InputContent(&e));	// Send Job if Event is changed
			break;
		default:
			break;
		}
	}
	_scheduler->addJob("Input", JOB_TYPES::INPUT_READ_CONTINUOUS);	// Read held keys
	return false;
}
