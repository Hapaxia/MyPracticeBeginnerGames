#include "Graphics.hpp"
#include <Plinth/Sfml/Generic.hpp>
#include <Plinth/Sfml/Colors.hpp>
#include "Player.hpp"
#include <Plinth/Sfml/Anchor.hpp>

Graphics::Graphics()
	: m_player({ 64.f, 32.f })
{
	m_player.setFillColor(pl::Colors::Cyan);
	m_player.setOrigin(pl::Anchor::Local::getBottomCenter(m_player));
}

void Graphics::updatePlayer(const sf::View& view, const Player& player)
{
	m_player.setPosition(static_cast<float>(player.getPosition()), view.getSize().y);
}

sf::Vector2f Graphics::getPlayerSize() const
{
	return m_player.getSize();
}

void Graphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_player);
}
