#ifndef MTD_BULLETS_HPP
#define MTD_BULLETS_HPP

#include "Bullet.hpp"
#include <list>

class Bullets
{
public:
	Bullets();
	void shoot(pl::Vector2d position);
	void reset();
	void update(double dt);
	void killBullet(unsigned int bulletIndex);
	std::list<Bullet>::const_iterator begin() const { return m_bullets.begin(); }
	std::list<Bullet>::const_iterator end() const { return m_bullets.end(); }
	std::size_t size() const { return m_bullets.size(); }

private:
	const double m_retriggerDelay;
	const double m_speed;
	std::list<Bullet> m_bullets;
	double m_retriggerTimer;
};

#endif // MTD_BULLETS_HPP
