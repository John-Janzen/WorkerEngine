#pragma once

#include "Scene.h"

class MainMenuScene :
	public Scene
{
public:
	MainMenuScene(Application * a, Render * r);
	~MainMenuScene();

	void InitScene();

	bool LoadScene();
	void UpdateScene();
	
private:

};

