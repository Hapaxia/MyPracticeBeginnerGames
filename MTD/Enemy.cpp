#include "Enemy.hpp"
#include <Plinth/Generic.hpp>

Enemy::Enemy(const sf::View& view, const pl::Vector2d position, pl::Vector2d size)
	: m_position(position)
	, m_size(size)
	, m_positionLimits({ pl::Range<double>{ 0 + m_size.x / 2.0, view.getSize().x - m_size.x / 2.0 }, { 0 + m_size.y / 2.0, view.getSize().y - m_size.y / 2.0 } })
	, m_isMovingRight(true)
	, m_requiresFlipping(false)
	, m_isAlive(true)
	, m_reachedBottom(false)
{
}

void Enemy::move(const pl::Vector2d movement)
{
	m_position += movement;
	if (!m_positionLimits.getHorizontalRange().contains(m_position.x))
		m_requiresFlipping = true;
	if (!m_positionLimits.getVerticalRange().contains(m_position.y))
		m_reachedBottom = true;
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

void Enemy::die()
{
	m_isAlive = false;
}

bool Enemy::isAlive() const
{
	return m_isAlive;
}

bool Enemy::reachedBottom() const
{
	return m_reachedBottom;
}