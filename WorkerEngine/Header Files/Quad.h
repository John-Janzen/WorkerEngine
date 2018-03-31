#pragma once
#include <glm.hpp>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <iostream>

#include "GameObject.h"

static const GLfloat vertex_positions[] = {
	-1.0f, -1.0f,  0.0f,
	1.0f, -1.0f,  0.0f,
	-1.0f,  1.0f,  0.0f,
	1.0f, 1.0f,  0.0f
};

// Indices for the triangle strips
static const GLuint vertex_indices[] = {
	0, 1, 2,
	2, 1, 3
};

class Quad : public GameObject
{
public:
	Quad(std::string, int);
	Quad(std::map<LOADABLE_ITEMS, std::string> s, std::vector<Component*> comp = std::vector<Component*>()) 
		: GameObject(s, comp) 
	{
		_velocity = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, 0);
	}
	void Update(float x = 0, float y = 0);
	~Quad();

private:
	glm::vec3 _velocity;
};

