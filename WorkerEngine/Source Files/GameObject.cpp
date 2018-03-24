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
		case POS:
		{
			std::string data = it->second;
			size_t loc;
			int count = 0;
			do
			{
				std::string sub = data.substr(0, (loc = data.find_first_of(',')));
				if (count == 0) x = atof(sub.c_str());
				else if (count == 1) y = atof(sub.c_str());
				else { z = atof(sub.c_str()); break; }
				data = data.substr(loc + 1);
				count++;
			} while (!data.empty());
		}
			break;
		default:
			break;
		}
	}
	_position = glm::vec3(x, y, z);
}

GameObject::~GameObject() {}
