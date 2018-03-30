#pragma once
#include <glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <time.h>

#include "RenderComponent.h"
#include "JTime.h"

enum LOADABLE_ITEMS
{
	DEFAULT, // Unused
	TYPE,
	NAME,
	ID,
	COMP,
	POS
};

class GameObject
{
public:
	GameObject() : _name("null"), _ID(0), _position{glm::vec3()} {};
	GameObject(std::string name, int id, glm::vec3 pos = glm::vec3());
	GameObject(std::map<LOADABLE_ITEMS, std::string> s, std::vector<Component*> comp = std::vector<Component*>());
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

	void changeColor()
	{
		collision = true;
	}

	virtual void Update(float x, float y) {};

	glm::vec4 color;

	int width = 2;
	int height = 2;

protected:
	glm::vec3 _position;
	bool collision = false;

private:
	int _ID;
	std::string _name;

	std::map<std::string, Component*> _components;
};

