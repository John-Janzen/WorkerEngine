#include "GameObject.h"

GameObject::GameObject(std::string name, int id, glm::vec3 pos) :
	_name(name), _ID(id), _position(pos)
{}

GameObject::GameObject(std::map<std::string, std::string> map)
{
	_name = map.find("name")->second;
	_ID = atoi(map.find("id")->second.c_str());
	std::string component = map.find("comp")->second;
	if (component.compare("render") == 0)
	{
		this->addComponent("Render", new RenderComponent());
	}
	int x = atoi(map.find("posX")->second.c_str());
	int y = atoi(map.find("posY")->second.c_str());
	int z = atoi(map.find("posZ")->second.c_str());
	_position = glm::vec3(x, y, z);
}

GameObject::~GameObject() {}
