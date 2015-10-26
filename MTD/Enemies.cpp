#include "Enemies.hpp"
#include <Plinth/Generic.hpp>

Enemies::Enemies(sf::RenderWindow& window)
	: m_speedIncreaseMultiplier(0.9)
	, m_speed(40.0)
	, m_enemies()
	, m_view(window.getView())
{
	priv_addEnemies();
}

void Enemies::reset()
{
	m_speed = 40.0;
	m_enemies.clear();
	priv_addEnemies();
}

void Enemies::update(double dt)
{
	bool requiresDirectionFlipping = false;
	for (auto& enemy : m_enemies)
	{
		if (!enemy.isAlive())
			continue;
		if (enemy.isMovingRight())
			enemy.move({ m_speed * dt, 0.0 });
		else
			enemy.move({ -m_speed * dt, 0.0 });
		if (enemy.requiresFlipping())
			requiresDirectionFlipping = true;
	}
	if (requiresDirectionFlipping)
		toggleDirection();
}

void Enemies::killEnemy(const unsigned int enemyIndex)
{
	if (enemyIndex < m_enemies.size() && m_enemies[enemyIndex].isAlive())
	{
		m_enemies[enemyIndex].die();

		// adjust speed following a kill
		if (getNumberOfEnemiesAlive() > 0)
			m_speed *= m_speedIncreaseMultiplier / getNumberOfEnemiesAlive() + 1;
	}
}

void Enemies::toggleDirection()
{
	for (auto& enemy : m_enemies)
		enemy.flipDirection();
}

unsigned int Enemies::getNumberOfEnemiesAlive() const
{
	unsigned int total{ 0u };
	for (auto& enemy : m_enemies)
	{
		if (enemy.isAlive())
			++total;
	}
	return total;
}

void Enemies::priv_addEnemies()
{
	const unsigned int numberOfEnemies = 40u;
	const unsigned int enemiesPerRow = 8u;
	const double horizontalSpacing = 65.0;
	const double verticalSpacing = 65.0;
	const pl::Vector2d positionOfTopLeft = { 120.0, 80.0 };
	for (unsigned int i = 0; i < numberOfEnemies; ++i)
		m_enemies.emplace_back(Enemy(m_view, positionOfTopLeft + pl::Vector2d{ horizontalSpacing * (i % enemiesPerRow), verticalSpacing * (i / enemiesPerRow) }));
}
