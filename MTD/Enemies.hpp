#ifndef MTD_ENEMIES_HPP
#define MTD_ENEMIES_HPP

#include "Enemy.hpp"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

class Enemies
{
public:
	Enemies(sf::RenderWindow& window);
	void update(double dt);
	void killEnemy(unsigned int enemyIndex);
	std::vector<Enemy>::const_iterator begin() const { return m_enemies.begin(); }
	std::vector<Enemy>::const_iterator end() const { return m_enemies.end(); }
	std::size_t size() const { return m_enemies.size(); }

	void toggleDirection();

private:
	std::vector<Enemy> m_enemies;
	sf::View m_view;
};

#endif // MTD_ENEMIES_HPP
