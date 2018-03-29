#include "Player.h"

void Player::Update(float x, float y)
{
	_position += glm::vec3(x, y, 0);
}