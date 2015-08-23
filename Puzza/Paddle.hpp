#ifndef PUZZA_PADDLE_HPP
#define PUZZA_PADDLE_HPP

#include <Plinth/Generic.hpp>

class Paddle
{
public:
	Paddle();
	void update(float dt);
	void setTargetPosition(float targetPosition);
	void setMaximumSpeed(float maximumSpeed);
	void setAcceleration(float acceleration);
	void setDeceleration(float deceleration);
	float getPosition() const;
	float getSpeed() const; // will be used to calculate how it contacts the ball

private:
	float m_speed; // current speed and direction of the paddle (negative is upwards)
	float m_maxSpeed; // maximum speed limits the top speed of the paddle
	float m_acceleration; // rate of speed increases
	float m_deceleration; // rate of speed decreases
	float m_position; // current position of the paddle (negative is upwards, zero is the centre, range is [-1 - 1])
	float m_targetPosition; // target position of the paddle - paddle moves automatically towards the target

	bool targetPositionReached() const;
};

#endif // PUZZA_PADDLE_HPP
