#include "Ball.hpp"

Ball::Ball():
m_position({ 0.f, 0.f }),
m_direction(0.f),
m_speed(0.f),
m_radius(10.f)
{
}

sf::Vector2f Ball::getPosition()
{
	return m_position;
}

float Ball::getDirection()
{
	return m_direction;
}

float Ball::getSpeed()
{
	return m_speed;
}

float Ball::getRadius()
{
	return m_radius;
}

void Ball::setPosition(sf::Vector2f position)
{
	m_position = position;
}

void Ball::setDirection(float direction)
{
	m_direction = pl::clampCycle(direction, pl::Range<float>{ 0.f, 360.f });
}

void Ball::changeSpeed(float speedChange)
{
	m_speed += speedChange;
}

void Ball::setSpeed(float speed)
{
	m_speed = speed;
}

void Ball::setRadius(float radius)
{
	m_radius = radius;
}

void Ball::flipDirectionHorizontally()
{
	setDirection(360.f - m_direction);
}

void Ball::flipDirectionVertically()
{
	setDirection(540.f - m_direction);
}

void Ball::moveForwards()
{
	const float radiansFromDegreesMultiplier = 0.0174532925f; // pi / 180 approx.
	const float directionInRadians = m_direction * radiansFromDegreesMultiplier;
	m_position += sf::Vector2f{ sin(directionInRadians) * m_speed, -cos(directionInRadians) * m_speed };
}
