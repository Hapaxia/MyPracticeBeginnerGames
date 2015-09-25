#include "Enemy.hpp"
#include <Plinth/Generic.hpp>

Enemy::Enemy(const sf::View& view, const pl::Vector2d position, pl::Vector2d size)
	: m_position(position)
	, m_size(size)
	, m_positionLimits({ 0 + m_size.x / 2.0, view.getSize().x - m_size.x / 2.0 })
	, m_isMovingRight(true)
	, m_requiresFlipping(false)
{
}

void Enemy::move(const pl::Vector2d movement)
{
	m_position += movement;
	if (!pl::inRange(m_position.x, m_positionLimits))
		m_requiresFlipping = true;
}

pl::Vector2d Enemy::getPosition() const
{
	return m_position;
}

pl::Vector2d Enemy::getSize() const
{
	return m_size;
}

bool Enemy::isMovingRight() const
{
	return m_isMovingRight;
}

void Enemy::flipDirection()
{
	m_requiresFlipping = false;
	pl::toggle(m_isMovingRight);
}

bool Enemy::requiresFlipping() const
{
	return m_requiresFlipping;
}
