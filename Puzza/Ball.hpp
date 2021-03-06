#ifndef PUZZA_BALL_HPP
#define PUZZA_BALL_HPP

#include <SFML/System/Vector2.hpp>

class Ball
{
public:
	Ball();
	void update(float dt);
	sf::Vector2f getPosition() const;
	float getDirection() const;
	float getSpeed() const;
	float getSpin() const;
	float getRadius() const;
	void setPosition(sf::Vector2f position);
	void setDirection(float direction);
	void setSpeed(float speed);
	void changeSpeed(float speedChange);
	void setSpin(float spin);
	void changeSpin(float spinChange);
	void setRadius(float radius);
	void flipDirectionHorizontally();
	void flipDirectionVertically();

private:
	const float m_maximumSpin;
	sf::Vector2f m_position;
	float m_direction;
	float m_speed;
	float m_radius;
	float m_spin;
};

#endif // PUZZA_BALL_HPP
