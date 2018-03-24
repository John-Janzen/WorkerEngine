#include "Quad.h"

Quad::Quad(std::string n, int id) : GameObject(n, id) {
	_position = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, 0);
	_velocity = glm::vec3(rand() % 4 - 2, rand() % 4 - 2, 0);
	color = glm::vec4(1.0, 1.0, 1.0, 1.0);
}


void Quad::Update(int boundX, int boundY)
{
	if (_position.x >= boundX || _position.x <= -boundX)
		_velocity.x = -_velocity.x;
	if (_position.y >= boundY || _position.y <= -boundY)
		_velocity.y = -_velocity.y;

	_position += _velocity * 0.1f;

	if (collision)
		color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	else
		color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	collision = false;
}

Quad::~Quad() {}