#include "Graphics.hpp"
#include <Plinth/Sfml/Generic.hpp>
#include <Plinth/Sfml/Colors.hpp>
#include "Player.hpp"
#include <Plinth/Sfml/Anchor.hpp>
#include "Bullets.hpp"
#include "Enemies.hpp"

#include <Dev.hpp>

#include <Plinth/StringFrom.hpp>

Graphics::Graphics()
	: m_player()
	, m_bullets()
	, m_enemies()
{
	m_player.setFillColor(pl::Colors::Cyan);
}

void Graphics::updateView(const sf::View& view)
{
	m_view = view;
}

void Graphics::updatePlayer(const Player& player)
{
	m_player.setSize(pl::Sfml::vector2(player.getSize()));
	m_player.setOrigin(pl::Anchor::Local::getBottomCenter(m_player));
	m_player.setPosition(static_cast<float>(player.getPosition()), m_view.getSize().y);
}

void Graphics::updateBullets(const Bullets& bullets)
{
	m_bullets.resize(bullets.size(), sf::RectangleShape());
	std::vector<sf::RectangleShape>::iterator it = m_bullets.begin();
	for (auto& bullet : bullets)
	{
		if (bullet.isAlive())
		{
			const sf::Vector2f bulletSize{ pl::Sfml::vector2(bullet.getSize()) };
			it->setSize(bulletSize);
			it->setFillColor(pl::Colors::LightYellow);
			it->setOrigin(pl::Anchor::Local::getBottomCenter(*it));
			it->setPosition(pl::Sfml::vector2(bullet.getPosition()));
			++it;
		}
		else
			m_bullets.pop_back();
	}
}

void Graphics::updateEnemies(const Enemies& enemies)
{
	m_enemies.resize(enemies.size(), sf::RectangleShape());
	std::vector<sf::RectangleShape>::iterator it = m_enemies.begin();
	const float angle{ static_cast<float>(enemies.getRotation()) };
	for (auto& enemy : enemies)
	{
		if (enemy.isAlive())
		{
			const sf::Vector2f enemySize{ pl::Sfml::vector2(enemy.getSize()) };
			it->setSize(enemySize);
			it->setFillColor(pl::Colors::Peach);
			it->setOrigin(pl::Anchor::Local::getCenter(*it));
			it->setPosition(pl::Sfml::vector2(enemy.getPosition()));
			it->setRotation(angle);
			++it;
		}
		else
			m_enemies.pop_back();
	}
}

void Graphics::clearBullets()
{
	m_bullets.clear();
}

void Graphics::clearEnemies()
{
	m_enemies.clear();
}

/*
sf::Vector2f Graphics::getPlayerSize() const
{
	return m_player.getSize();
}
*/

void Graphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& bullet : m_bullets)
		target.draw(bullet);
	for (auto& enemy : m_enemies)
		target.draw(enemy);
	target.draw(m_player);
}
