#ifndef PUZZA_BALL_HPP
#define PUZZA_BALL_HPP

#include <SFML/System/Vector2.hpp>
#include <math.h>
#include <Plinth/Generic.hpp>

class Ball
{
public:
	Ball();
	void moveForwards();
	sf::Vector2f getPosition();
	float getDirection();
	float getSpeed();
	float getRadius();
	void setPosition(sf::Vector2f position);
	void setDirection(float direction);
	void setSpeed(float speed);
	void changeSpeed(float speedChange);
	void setRadius(float radius);
	void flipDirectionHorizontally();
	void flipDirectionVertically();

private:
	sf::Vector2f m_position;
	float m_direction;
	float m_speed;
	float m_radius;
};

#endif // PUZZA_BALL_HPP
