#include "Enemy.hpp"
#include <Plinth/Generic.hpp>

namespace
{

constexpr double bulletDropSpeed{ 300.0 };
const pl::Vector2d bulletSize{ 8.0, 32.0 };

} // namespace

Enemy::Enemy(const sf::View& view, const pl::Vector2d position, const pl::Vector2d size)
	: m_position(position)
	, m_size(size)
	, m_positionLimits({ pl::Range<double>{ 0 + m_size.x / 2.0, view.getSize().x - m_size.x / 2.0 }, { 0 + m_size.y / 2.0, view.getSize().y - m_size.y / 2.0 } })
	, m_isMovingRight(true)
	, m_requiresFlipping(false)
	, m_isAlive(true)
	, m_reachedBottom(false)
	, m_bullet()
	, m_isBulletAlive(false)
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

void Enemy::shoot()
{
	if (m_isBulletAlive)
		return;
	m_isBulletAlive = true;
	m_bullet = m_position;
	m_bullet.y += (bulletSize.y + m_size.y) / 2.0;
}

void Enemy::updateBullet(const double dt)
{
	m_bullet.y += dt * bulletDropSpeed;
	if (!m_positionLimits.getVerticalRange().contains(m_bullet.y - (bulletSize.y + m_size.y) / 2.0))
		m_isBulletAlive = false;
}

pl::Vector2d Enemy::getPosition() const
{
	return m_position;
}

pl::Vector2d Enemy::getSize() const
{
	return m_size;
}

pl::Vector2d Enemy::getBulletPosition() const
{
	return m_bullet;
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

bool Enemy::isBulletAlive() const
{
	return m_isBulletAlive;
}
