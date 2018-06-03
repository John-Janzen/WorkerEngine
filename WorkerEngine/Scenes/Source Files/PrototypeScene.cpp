#include "PrototypeScene.h"
#include "../../Header Files/Application.h"
#include "../../Header Files/Render.h"

PrototypeScene::PrototypeScene(Application * a) : Scene(a) 
{
	a->addJob("FileLoader", FILE_LOAD_EXTERNAL, new FileToLoadContent("Assets/Prototype.dat", 0));
}

PrototypeScene::~PrototypeScene() {}

bool PrototypeScene::LoadScene(GameObject *& camera) 
{
	if (_sceneObjects.size() == numOfObjects && Manager::instance().checkDone())
	{
		bool _flag = false;
		printf("Loading started\n");
		for (GameObject * obj : _sceneObjects)
		{
			if (obj->getName().compare("Camera") == 0)
			{
				camera = obj;
				return true;
			}
		}
	}
	return false;
}

void PrototypeScene::UpdateScene()
{
	Scene::UpdateScene();

	for (GameObject * go : _sceneObjects)
		app->addJob("Engine", ENGINE_HANDLE_OBJECT, new EngineObjectContent(go));
}

void PrototypeScene::ReadInputs()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))// Listen for events
	{
		switch (e.type)
		{
		case SDL_QUIT:		// Quit the game
			app->Quit();
			break;
		case SDL_KEYDOWN:
			if (e.key.repeat == 0)
				app->addJob("Input", JOB_TYPES::INPUT_READ_PRESSED, new InputContent(e));	// Send Job if Event is changed
			break;
		default:
			break;
		}
	}
	app->addJob("Input", JOB_TYPES::INPUT_READ_CONTINUOUS);	// Read held keys
}
