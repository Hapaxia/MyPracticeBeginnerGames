#include "Paddle.hpp"

Paddle::Paddle():
m_speed(0.f),
m_maxSpeed(10.f),
m_acceleration(30.f),
m_deceleration(1000.f),
m_position(0.f),
m_targetPosition(0.f)
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
	m_speed = pl::clamp(m_speed, pl::Range<float>{ -m_maxSpeed, m_maxSpeed });

	// move
	m_position += m_speed * dt;

	// clamp position
	m_position = pl::clamp(m_position, pl::Range<float>{ -1.f, 1.f });
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

bool Paddle::targetPositionReached() const
{
	const float reachExtremity{ 0.01f };
	const float offset{ m_targetPosition - m_position };
	return (offset > -reachExtremity) && (offset < reachExtremity);
}