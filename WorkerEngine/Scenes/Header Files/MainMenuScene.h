#pragma once

#include "Scene.h"

class MainMenuScene :
	public Scene
{
public:
	MainMenuScene();
	MainMenuScene(Application * a);
	~MainMenuScene();

	bool LoadScene(GameObject *& camera);
	void UpdateScene();

	void ReadInputs();
	
private:
};

