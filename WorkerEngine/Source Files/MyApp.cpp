#include "MyApp.h"

MyApp::MyApp() {}

MyApp::~MyApp() {}

void MyApp::Init(uint16_t n)
{
	Application::Init(n);
	printf("Time Load Start: %ums\n", SDL_GetTicks());
	renderCopy->Update(RENDER_INIT, _flag);
	Manager::instance().addJob("FileLoader", FILE_LOAD_TXT_DATA, new FileToLoadContent("Assets/prototype.dat"));
	/*for (int i = 0; i < 100; i++)
	{
		std::string name("Quad_" + std::to_string(i));
		Manager::instance().addJob("Application", APPLICATION_ADD_SINGLE_OBJECT, new FileLoadedContent(new Quad(name, i)));
	}*/
}

void MyApp::Update()
{
	now = SDL_GetTicks();
	switch (state)
	{
	case LOADING:
		Manager::instance();
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
		for (GameObject * go : _worldObjects)
			go->Update(50, 50);

		while (Manager::instance().checkBusy());			// Wait for the threads to finish

		renderCopy->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(&_worldObjects));		// Render the screen
		frameTicks = SDL_GetTicks();
		//printf("%u-", frameTicks - now);
		break;
	}
	default:
		break;
	}
}
