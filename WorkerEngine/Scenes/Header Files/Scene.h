#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

#include "../../Header Files/GameObject.h"

enum STATE
{
	LOADING,
	UPDATE
};

class Application;
class Scene
{
public:
	Scene(Application * a) : app{ a } {}
	virtual ~Scene() {}
	virtual void InitScene() = 0;
	virtual bool LoadScene(GameObject *&) = 0;
	virtual void UpdateScene() { ReadInputs(); };
	virtual void ReadInputs() {};

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

	const std::vector<GameObject*> & getSceneObjects()
	{
		return _sceneObjects;
	}

protected:
	std::vector<GameObject*> _sceneObjects;
	int numOfObjects = -1;
	GameObject * _cameraObject;
	Application * app;
};

