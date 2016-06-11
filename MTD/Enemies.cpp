#include "Enemies.hpp"
#include <Plinth/Generic.hpp>
#include <Plinth/Range.hpp>

Enemies::Enemies(sf::RenderWindow& window)
	: m_speedIncreaseMultiplier(0.9)
	, m_dropSpeedIncreaseMultiplier(1.1)
	, m_rotationSpeed(10.0)
	, m_speed(40.0)
	, m_dropSpeed(0.05)
	, m_rotation(0.0)
	, m_rotationRange({ 0.0, 0.0 })
	, m_enemies()
	, m_view(window.getView())
	, m_reachedBottom(false)
{
	priv_addEnemies();
}

void Enemies::reset()
{
	m_speed = 40.0;
	m_dropSpeed = 0.05;
	m_reachedBottom = false;
	m_enemies.clear();
	priv_addEnemies();
}

void Enemies::update(double dt)
{
	if (m_reachedBottom)
		return;

	bool requiresDirectionFlipping = false;
	for (auto& enemy : m_enemies)
	{
		if (!enemy.isAlive())
			continue;

		if (enemy.isMovingRight())
			enemy.move({ m_speed * dt, m_speed * m_dropSpeed * dt });
		else
			enemy.move({ -m_speed * dt, m_speed * m_dropSpeed * dt });
		if (enemy.requiresFlipping())
			requiresDirectionFlipping = true;
		if (enemy.reachedBottom())
			m_reachedBottom = true;
	}
	if (requiresDirectionFlipping)
		toggleDirection();
	const float targetAngle{ static_cast<float>((m_speed - 40.0) * 0.035) }; // angle to 'lean' based on current speed
	m_rotationRange.set(-targetAngle, targetAngle);
	if (m_enemies[0].isMovingRight())
		m_rotation += targetAngle * m_rotationSpeed * dt;
	else
		m_rotation -= targetAngle * m_rotationSpeed * dt;
	m_rotation = m_rotationRange.clamp(m_rotation);
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
	m_dropSpeed *= m_dropSpeedIncreaseMultiplier;
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

bool Enemies::getReachedBottom() const
{
	return m_reachedBottom;
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