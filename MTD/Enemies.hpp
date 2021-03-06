#ifndef MTD_ENEMIES_HPP
#define MTD_ENEMIES_HPP

#include "Enemy.hpp"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Plinth/Range.hpp>

class Enemies
{
public:
	Enemies(sf::RenderWindow& window);
	void reset();
	void update(double dt, double targetPosition);
	void killEnemy(unsigned int enemyIndex);
	std::vector<Enemy>::const_iterator begin() const { return m_enemies.begin(); }
	std::vector<Enemy>::const_iterator end() const { return m_enemies.end(); }
	std::vector<Enemy>::iterator begin() { return m_enemies.begin(); }
	std::vector<Enemy>::iterator end() { return m_enemies.end(); }
	std::size_t size() const { return m_enemies.size(); }

	void toggleDirection();
	unsigned int getNumberOfEnemiesAlive() const;
	bool getReachedBottom() const;

	double getSpeed() const { return m_speed; }
	double getDropSpeed() const { return m_dropSpeed; }
	double getRotation() const { return m_rotation; }

private:
	const double m_speedIncreaseMultiplier;
	const double m_dropSpeedIncreaseMultiplier;
	const double m_rotationSpeed;
	double m_speed;
	double m_dropSpeed;
	double m_rotation;
	pl::Range<double> m_rotationRange;
	std::vector<Enemy> m_enemies;
	sf::View m_view;
	bool m_reachedBottom;
	double m_targetPosition;

	void priv_addEnemies();
};

#endif // MTD_ENEMIES_HPP
