#ifndef PUZZA_SCORES_HPP
#define PUZZA_SCORES_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Scores : public sf::Drawable
{
public:
	Scores(const sf::RenderWindow& window);
	void updatePlayer(unsigned int playerScore);
	void updateOpponent(unsigned int opponentScore);
	void setFont(const sf::Font& font);

private:
	sf::Text m_player;
	sf::Text m_opponent;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PUZZA_SCORES_HPP
