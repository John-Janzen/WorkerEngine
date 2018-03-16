#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <string>

class GameObject
{
public:
	GameObject(std::string name, int id, glm::vec3 pos = glm::vec3());
	~GameObject();

	void adjustPosY(GLfloat num)
	{
		_position.y += num;
	}

	void adjustPosX(GLfloat num)
	{
		_position.x += num;
	}

	glm::vec3 getPos()
	{
		return _position;
	}

private:
	int _ID;
	std::string _name;
	glm::vec3 _position;
};

