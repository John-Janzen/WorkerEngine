#include "GameObject.h"

GameObject::GameObject(std::string name, int id, glm::vec3 pos) :
	_name(name), _ID(id), _position(pos)
{}

GameObject::~GameObject() {}
