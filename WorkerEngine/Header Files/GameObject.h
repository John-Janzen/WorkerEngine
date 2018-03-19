#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <string>
#include <map>

#include "RenderComponent.h"

class GameObject
{
public:
	GameObject() { _name = "null"; _ID = 0; _position = glm::vec3(); };
	GameObject(std::string name, int id, glm::vec3 pos = glm::vec3());
	GameObject(std::map<std::string, std::string> s);
	~GameObject();

	void adjustPosY(GLfloat num) { _position.y += num; }
	void adjustPosX(GLfloat num) { _position.x += num; }
	void adjustPosZ(GLfloat num) { _position.z += num; }

	void setName(std::string name) { _name = name; };
	void setID(int id) { _ID = id; };
	void setPos(glm::vec3 pos) { _position = pos; };

	glm::vec3 getPos() { return _position; }
	std::string getName() { return _name; };

	void addComponent(const std::string & name, Component * component)
	{
		_components.emplace(std::make_pair(name, component));
	}

	Component* getComponent(const std::string & name)
	{
		if (_components.find(name) == _components.end())
			return nullptr;
		else
			return _components[name];
	}

private:
	int _ID;
	std::string _name;
	glm::vec3 _position;

	std::map<std::string, Component*> _components;
};

