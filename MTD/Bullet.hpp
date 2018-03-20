#ifndef MTD_BULLET_HPP
#define MTD_BULLET_HPP

#include <Plinth/Vector2.hpp>

class Bullet
{
public:
	Bullet(pl::Vector2d position, pl::Vector2d size = { 4.0, 32.0 });
	void move(double movement);
	pl::Vector2d getPosition() const;
	pl::Vector2d getSize() const;
	void die();
	bool isAlive() const;

private:
	const pl::Vector2d m_size;
	pl::Vector2d m_position;
	bool m_isAlive;
};

#endif // MTD_BULLET_HPP
