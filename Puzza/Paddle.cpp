#include "Paddle.hpp"

#include <Plinth/Generic.hpp>
#include <Plinth/Range.hpp>

Paddle::Paddle():
m_speed(0.f),
m_maxSpeed(10.f),
m_acceleration(1000.f),
m_deceleration(10000.f),
m_position(0.f),
m_targetPosition(0.f),
m_score(0u)
{

}

void Paddle::update(float dt)
{
	// calculate speed
	if (targetPositionReached())
		m_speed = 0.f;
	else if (m_targetPosition > m_position)
	{
		if (m_speed < 0.f) // travelling away from target
		{
			m_speed += m_deceleration * dt;
			if (m_speed > 0.f)
				m_speed = 0.f;
		}
		else
			m_speed += m_acceleration * dt;
	}
	else if (m_targetPosition < m_position)
	{
		if (m_speed > 0.f) // travelling away from target
		{
			m_speed -= m_deceleration * dt;
			if (m_speed < 0.f)
				m_speed = 0.f;
		}
		else
			m_speed -= m_acceleration * dt;

	}

	// clamp speed
	m_speed = pl::Range<float>{ -m_maxSpeed, m_maxSpeed }.clamp(m_speed);

	// move
	m_position += m_speed * dt;

	// clamp position
	if (!pl::Range<float>{ -1.f, 1.f }.contains(m_position))
	{
		m_position = pl::Range<float>{ -1.f, 1.f }.clamp(m_position);
		m_speed = 0.f;
	}
}

void Paddle::setTargetPosition(float targetPosition)
{
	m_targetPosition = targetPosition;
}

void Paddle::setMaximumSpeed(float maximumSpeed)
{
	m_maxSpeed = maximumSpeed;
}

void Paddle::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}

void Paddle::setDeceleration(float deceleration)
{
	m_deceleration = deceleration;
}

float Paddle::getPosition() const
{
	return m_position;
}

float Paddle::getSpeed() const
{
	return m_speed;
}

void Paddle::resetScore()
{
	m_score = 0u;
}

void Paddle::increaseScore()
{
	++m_score;
}

unsigned int Paddle::getScore() const
{
	return m_score;
}

bool Paddle::targetPositionReached() const
{
	const float reachExtremity{ 0.01f };
	const float offset{ m_targetPosition - m_position };
	return (offset > -reachExtremity) && (offset < reachExtremity);
}
