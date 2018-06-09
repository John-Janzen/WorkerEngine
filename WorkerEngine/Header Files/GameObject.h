#pragma once
#include <glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

#include "RenderComponent.h"
#include "JTime.h"

enum LOADABLE_ITEMS
{
	DEFAULT, // Unused
	TYPE,
	NAME,
	ID,
	COMP,
	POS,
	ROT,
};

class GameObject
{
public:
	GameObject() : _name("null"), _ID(0), _position{ glm::vec3() } {};
	GameObject(std::string name, int id, glm::vec3 pos = glm::vec3());
	GameObject(std::map<LOADABLE_ITEMS, std::string> s, std::vector<Component*> comp = std::vector<Component*>());
	~GameObject();

	void DrawMyself(const float * matrix);

	void adjustPosY(GLfloat num) { _position.y += num; }
	void adjustPosX(GLfloat num) { _position.x += num; }
	void adjustPosZ(GLfloat num) { _position.z += num; }

	void setName(std::string name) { _name = name; };
	void setID(int id) { _ID = id; };
	void setPos(glm::vec3 pos) { _position = pos; };
	void setRot(glm::vec3 rot) { _rotation = rot; };

	glm::vec3 getPos() { return _position; }
	glm::vec3 getRot() { return _rotation; }
	std::string getName() { return _name; };

	void addComponent(Component * component)
	{
		_components[component->getID()] = component;
	}

	template <class T>
	T getComponent()
	{
		return (this->hasComponent<T>()) ? dynamic_cast<T>(_components[this->getType<T>()]) : nullptr;
	}

	template <class T>
	bool hasComponent()
	{
		return (_components.find(this->getType<T>()) != _components.end()) ? true : false;
	}

	virtual void Update(float x, float y) {};

	int width = 2;
	int height = 2;

protected:
	glm::vec3 _position;
	glm::vec3 _rotation;
	bool collision = false;

private:
	int _ID;
	std::string _name;

	template <typename T>
	std::string getType()
	{
		return typeid(T).name();
	};

	std::map<std::string, Component*> _components;
	//std::vector<Component*> _components;
};
