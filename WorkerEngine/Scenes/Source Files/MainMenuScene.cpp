
#include "MainMenuScene.h"
#include "../../Header Files/Application.h"
#include "../../Header Files/Render.h"

MainMenuScene::MainMenuScene(Application * a, Render * r) : Scene(a, r) {}

MainMenuScene::~MainMenuScene() { app = nullptr; DestroyObjects(); }

void MainMenuScene::InitScene()
{
	app->addJob("FileLoader", FILE_LOAD_EXTERNAL, new FileToLoadContent("Assets/MainMenu.dat"));
}

bool MainMenuScene::LoadScene()
{
	if (_sceneObjects.size() == numOfObjects && !Manager::instance().checkDone())
	{
		bool _flag = false;
		printf("Loading started\n");
		for (GameObject * obj : _sceneObjects)
		{
			if (obj->getName().compare("Camera") == 0)
			{
				_cameraObject = obj;
				renderCopy->Update(RENDER_LOAD, _flag, new RenderLoadContent(&_sceneObjects, _cameraObject));
				return true;
			}
		}
	}
	return false;
}

void MainMenuScene::UpdateScene()
{
	bool _flag = false;
	for (GameObject * go : _sceneObjects)
		app->addJob("Engine", ENGINE_HANDLE_OBJECT, new EngineObjectContent(go));

	while (Manager::instance().checkDone());			// Wait for the threads to finish

	renderCopy->Update(RENDER_UPDATE, _flag, new RenderUpdateContent(&_sceneObjects, _cameraObject));
}
