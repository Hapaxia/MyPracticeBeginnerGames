#include "Player.hpp"
#include <Plinth/Sfml/Generic.hpp>

namespace
{

const pl::Vector2d playerSize{ 64.0, 32.0 };
constexpr double initialSpeed{ 300.0 };
const double minimumPosition{ 0.0 + playerSize.x / 2.0 };

}

Player::Player(const sf::RenderWindow& window, const double dt, const Graphics& graphics)
	: m_dt(dt)
	, m_speed(initialSpeed)
	, m_size(playerSize)
	, m_positionLimits({ minimumPosition, window.getSize().x - playerSize.x / 2.0 })
	, m_position(m_positionLimits.min)
{
}

void Player::move(const Direction direction)
{
	double movement;
	switch (direction)
	{
	case Direction::Left:
		movement = -1.0;
		break;
	case Direction::Right:
		movement = 1.0;
		break;
	default:
		movement = 0.0;
	}
	m_position += movement * m_speed * m_dt;
	m_position = m_positionLimits.clamp(m_position);
}

void Player::reset()
{
	m_position = minimumPosition;
	m_speed = initialSpeed;
}

double Player::getPosition() const
{
	return m_position;
}

pl::Vector2d Player::getSize() const
{
	return m_size;
}
