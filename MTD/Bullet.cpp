#include "Bullet.hpp"

Bullet::Bullet(const pl::Vector2d position)
	: m_position(position)
{
}

void Bullet::update(const double movement)
{
	m_position.y -= movement;
}

pl::Vector2d Bullet::getPosition() const
{
	return m_position;
}
