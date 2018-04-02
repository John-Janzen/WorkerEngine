#pragma once
#include "Scene.h"
class PrototypeScene : public Scene
{
public:
	PrototypeScene(Application * a);
	~PrototypeScene();
	void InitScene();
	bool LoadScene(GameObject *&);
	void UpdateScene();
	void ReadInputs();
private:

};

