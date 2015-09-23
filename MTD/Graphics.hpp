#ifndef MTD_GRAPHICS_HPP
#define MTD_GRAPHICS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class Player;
class Bullets;

class Graphics : public sf::Drawable
{
public:
	Graphics();
	void updatePlayer(const sf::View& view, const Player& player);
	void updateBullets(const sf::View& view, const Bullets& bullets);
	sf::Vector2f getPlayerSize() const;

private:
	sf::RectangleShape m_player;
	std::vector<sf::RectangleShape> m_bullets;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // MTD_GRAPHICS_HPP
