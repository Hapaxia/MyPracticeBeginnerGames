#include "Graphics.hpp"
#include <Plinth/Sfml/Generic.hpp>
#include <Plinth/Sfml/Colors.hpp>
#include "Player.hpp"
#include <Plinth/Sfml/Anchor.hpp>
#include "Bullets.hpp"

#include <Dev.hpp>

#include <Plinth/StringFrom.hpp>

Graphics::Graphics()
	: m_player({ 64.f, 32.f })
	, m_bullets()
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
		it->setOrigin(pl::Anchor::Local::getBottomCenter(*it));
		it->setPosition(pl::Sfml::vector2(bullet.getPosition()));
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
	target.draw(m_player);
}
