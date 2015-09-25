#ifndef MTD_BULLET_HPP
#define MTD_BULLET_HPP

#include <Plinth/Generic.hpp>

class Bullet
{
public:
	Bullet(pl::Vector2d position);
	void move(double movement);
	pl::Vector2d getPosition() const;

private:
	pl::Vector2d m_position;
};

#endif // MTD_BULLET_HPP
