#include "Quad.h"

Quad::Quad(std::string n, int id) : GameObject(n, id) {
	_position = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, 0);
	_velocity = glm::vec3(rand() % 4 - 2, rand() % 4 - 2, 0);
}


void Quad::Update(float x, float y)
{
	int boundX = 50, boundY = 50;
	if (_position.x >= boundX || _position.x <= -boundX)
		_velocity.x = -_velocity.x;
	if (_position.y >= boundY || _position.y <= -boundY)
		_velocity.y = -_velocity.y;

	/*_position += _velocity * 10.0f * JTime::instance().deltaTime;
	_rotation += glm::vec3(0, 0, 1.57) * JTime::instance().deltaTime;*/
}

Quad::~Quad() {}