#include "Scores.hpp"

#include <Plinth/StringFrom.hpp>
#include <Plinth/Sfml/Anchor.hpp>

Scores::Scores(const sf::RenderWindow& window) :
m_player(),
m_opponent()
{
	m_player.setString("0");
	m_opponent.setString("0");
	m_player.setCharacterSize(60u);
	m_opponent.setCharacterSize(60u);
	m_player.setPosition({ window.getSize().x * 0.25f, 10.f });
	m_opponent.setPosition({ window.getSize().x * 0.75f, 10.f });
}

void Scores::updatePlayer(unsigned int playerScore)
{
	m_player.setString(pl::stringFrom(playerScore));
	m_player.setOrigin(pl::Anchor::Local::getTopCenter(m_player));
}

void Scores::updateOpponent(unsigned int opponentScore)
{
	m_opponent.setString(pl::stringFrom(opponentScore));
	m_opponent.setOrigin(pl::Anchor::Local::getTopCenter(m_opponent));
}

void Scores::setFont(const sf::Font& font)
{
	m_player.setFont(font);
	m_opponent.setFont(font);
}

void Scores::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_player);
	target.draw(m_opponent);
}
