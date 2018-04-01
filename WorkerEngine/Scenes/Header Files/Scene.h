#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

#include "../../Header Files/GameObject.h"

class Application;
class Render;
class Scene
{
public:
	Scene(Application * a, Render * r) : app{ a }, renderCopy{ r } {}
	virtual ~Scene() {}
	virtual void InitScene() = 0;
	virtual bool LoadScene() = 0;
	virtual void UpdateScene() = 0;

	void AddObject(GameObject * go)
	{
		_sceneObjects.emplace_back(go);
	}

	void initNumberObjects(int num)
	{
		numOfObjects = num;
	}

	void DestroyObjects()
	{
		for (GameObject * go : _sceneObjects)
			delete(go);

		_sceneObjects.clear();
	}

protected:
	std::vector<GameObject*> _sceneObjects;
	int numOfObjects = -1;
	GameObject * _cameraObject;
	Application * app;
	Render * renderCopy;
};

