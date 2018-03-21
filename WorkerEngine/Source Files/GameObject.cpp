#include "GameObject.h"

GameObject::GameObject(std::string name, int id, glm::vec3 pos) :
	_name(name), _ID(id), _position(pos)
{}

GameObject::GameObject(std::map<std::string, std::string> map, std::vector<Component*> comp)
{
	_name = map.find("name")->second;
	_ID = atoi(map.find("id")->second.c_str());

	if (map.find("comp") != map.end())
		std::string component = map.find("comp")->second;

	GLfloat x = atof(map.find("posX")->second.c_str());
	GLfloat y = atof(map.find("posY")->second.c_str());
	GLfloat z = atof(map.find("posZ")->second.c_str());
	_position = glm::vec3(x, y, z);
	for (Component * c : comp)
	{
		this->addComponent(map.find("comp")->second, c);
	}
}

GameObject::~GameObject() {}
