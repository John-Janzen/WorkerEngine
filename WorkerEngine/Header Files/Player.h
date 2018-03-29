#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:

	Player(std::map<LOADABLE_ITEMS, std::string> s, std::vector<Component*> comp = std::vector<Component*>())
		: GameObject(s, comp) 
	{
		color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	~Player() {}
	void Update(float x = 0, float y = 0);

private:

};

