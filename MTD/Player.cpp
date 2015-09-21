#include "Player.hpp"
#include <Plinth/Sfml/Generic.hpp>

Player::Player(const sf::RenderWindow& window, double dt, const Graphics& graphics)
	: m_dt(dt)
	, m_speed(300.0)
	, m_positionLimits({ 0 + graphics.getPlayerSize().x / 2.f, window.getSize().x - graphics.getPlayerSize().x / 2.f })
	, m_position(m_positionLimits.min)
{
}

void Player::move(Direction direction)
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
	m_position = pl::clamp(m_position, m_positionLimits);
}

double Player::getPosition() const
{
	return m_position;
}
