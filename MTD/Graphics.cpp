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
	: m_player({ 64.f, 32.f })
	, m_bullets()
	, m_enemies()
{
	m_player.setFillColor(pl::Colors::Cyan);
	m_player.setOrigin(pl::Anchor::Local::getBottomCenter(m_player));
}

void Graphics::updateView(const sf::View& view)
{
	m_view = view;
}

void Graphics::updatePlayer(const Player& player)
{
	m_player.setPosition(static_cast<float>(player.getPosition()), m_view.getSize().y);
}

void Graphics::updateBullets(const Bullets& bullets)
{
	const sf::Vector2f bulletSize{ 8.f, 24.f };
	m_bullets.resize(bullets.size(), sf::RectangleShape(bulletSize));
	std::vector<sf::RectangleShape>::iterator it = m_bullets.begin();
	for (auto& bullet : bullets)
	{
		it->setFillColor(pl::Colors::LightYellow);
		it->setOrigin(pl::Anchor::Local::getBottomCenter(*it));
		it->setPosition(pl::Sfml::vector2(bullet.getPosition()));
		++it;
	}
}

void Graphics::updateEnemies(const Enemies& enemies)
{
	m_enemies.resize(enemies.size(), sf::RectangleShape());
	std::vector<sf::RectangleShape>::iterator it = m_enemies.begin();
	for (auto& enemy : enemies)
	{
		const sf::Vector2f enemySize{ pl::Sfml::vector2(enemy.getSize()) };
		it->setSize(enemySize);
		it->setFillColor(pl::Colors::Peach);
		it->setOrigin(pl::Anchor::Local::getCenter(*it));
		it->setPosition(pl::Sfml::vector2(enemy.getPosition()));
		++it;
	}
}

sf::Vector2f Graphics::getPlayerSize() const
{
	return m_player.getSize();
}

void Graphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& bullet : m_bullets)
		target.draw(bullet);
	for (auto& enemy : m_enemies)
		target.draw(enemy);
	target.draw(m_player);
}
