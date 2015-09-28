#include "Enemies.hpp"
#include <Plinth/Generic.hpp>

Enemies::Enemies(sf::RenderWindow& window)
	: m_enemies()
	, m_view(window.getView())
{
	const unsigned int numberOfEnemies = 40u;
	const unsigned int enemiesPerRow = 8u;
	const double horizontalSpacing = 65.0;
	const double verticalSpacing = 65.0;
	const pl::Vector2d positionOfTopLeft = { 120.0, 80.0 };
	for (unsigned int i = 0; i < numberOfEnemies; ++i)
		m_enemies.emplace_back(Enemy(m_view, positionOfTopLeft + pl::Vector2d{ horizontalSpacing * (i % enemiesPerRow), verticalSpacing * (i / enemiesPerRow) }));
}

void Enemies::update(double dt)
{
	bool requiresDirectionFlipping = false;
	const double enemySpeed = 50.0;
	for (auto& enemy : m_enemies)
	{
		if (!enemy.isAlive())
			continue;
		if (enemy.isMovingRight())
			enemy.move({ enemySpeed * dt, 0.0 });
		else
			enemy.move({ -enemySpeed * dt, 0.0 });
		if (enemy.requiresFlipping())
			requiresDirectionFlipping = true;
	}
	if (requiresDirectionFlipping)
		toggleDirection();
}

void Enemies::killEnemy(const unsigned int enemyIndex)
{
	if (enemyIndex < m_enemies.size())
		m_enemies[enemyIndex].die();
}

void Enemies::toggleDirection()
{
	for (auto& enemy : m_enemies)
		enemy.flipDirection();
}