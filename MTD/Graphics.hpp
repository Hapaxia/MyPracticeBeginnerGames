#ifndef MTD_GRAPHICS_HPP
#define MTD_GRAPHICS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class Player;

class Graphics : public sf::Drawable
{
public:
	Graphics();
	void updatePlayer(const sf::View& view, const Player& player);
	sf::Vector2f getPlayerSize() const;

private:
	sf::RectangleShape m_player;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // MTD_GRAPHICS_HPP
