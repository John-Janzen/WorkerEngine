#include "GameObject.h"

GameObject::GameObject(std::string name, int id, glm::vec3 pos) :
	_name(name), _ID(id), _position(pos)
{}

GameObject::GameObject(std::map<LOADABLE_ITEMS, std::string> map, std::vector<Component*> comp)
{
	GLfloat x = 0, y = 0, z = 0;
	for (std::map<LOADABLE_ITEMS, std::string>::iterator it = map.begin(); it != map.end(); ++it)
	{
		switch (it->first)
		{
		case NAME:
			_name = it->second;
			break;
		case COMP:
		{
			for (Component * c : comp)
			{
				this->addComponent(it->second, c);
			}
			break;
		}
		case ID:
			_ID = atoi(it->second.c_str());
			break;
		case POSX:
			x = atof(it->second.c_str());
			break;
		case POSY:
			y = atof(it->second.c_str());
			break;
		case POSZ:
			z = atof(it->second.c_str());
			break;
		default:
			break;
		}
	}
	_position = glm::vec3(x, y, z);
}

GameObject::~GameObject() {}
