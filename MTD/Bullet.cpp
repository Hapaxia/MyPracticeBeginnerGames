#include "Bullet.hpp"

Bullet::Bullet(const pl::Vector2d position, const pl::Vector2d size)
	: m_position(position)
	, m_size(size)
	, m_isAlive(true)
{
}

void Bullet::move(const double movement)
{
	m_position.y -= movement;
}

pl::Vector2d Bullet::getPosition() const
{
	return m_position;
}

pl::Vector2d Bullet::getSize() const
{
	return m_size;
}

void Bullet::die()
{
	m_isAlive = false;
}

bool Bullet::isAlive() const
{
	return m_isAlive;
}
