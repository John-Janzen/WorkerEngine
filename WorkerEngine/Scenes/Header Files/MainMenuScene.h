#pragma once

#include "Scene.h"

class MainMenuScene :
	public Scene
{
public:
	MainMenuScene(Application * a);
	~MainMenuScene();

	void InitScene();

	bool LoadScene(GameObject *& camera);
	void UpdateScene();

	void ReadInputs();
	
private:
};

