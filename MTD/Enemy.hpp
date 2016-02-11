#ifndef MTD_ENEMY_HPP
#define MTD_ENEMY_HPP

#include <Plinth/Generic.hpp>
#include <SFML/Graphics/View.hpp>

class Enemy
{
public:
	Enemy(const sf::View& view, pl::Vector2d position, pl::Vector2d size = { 48.0, 32.0 });
	void move(pl::Vector2d movement);
	pl::Vector2d getPosition() const;
	pl::Vector2d getSize() const;
	bool isMovingRight() const;
	void flipDirection();
	bool requiresFlipping() const;
	void die();
	bool isAlive() const;
	bool reachedBottom() const;

private:
	const pl::Vector2d m_size;
	pl::Vector2d m_position;
	pl::AreaRange<double> m_positionLimits;
	bool m_isMovingRight;
	bool m_requiresFlipping;
	bool m_isAlive;
	bool m_reachedBottom;
};

#endif // MTD_ENEMY_HPP
