#ifndef MTD_GRAPHICS_HPP
#define MTD_GRAPHICS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>

class Player;
class Bullets;
class Enemies;

class Graphics : public sf::Drawable
{
public:
	Graphics();
	void updateView(const sf::View& view);
	void updatePlayer(const Player& player);
	void updateBullets(const Bullets& bullets);
	void updateEnemies(const Enemies& enemies);
	void clearBullets();
	void clearEnemies();
	//sf::Vector2f getPlayerSize() const;

private:
	sf::RectangleShape m_player;
	std::vector<sf::RectangleShape> m_bullets;
	std::vector<sf::RectangleShape> m_enemies;
	sf::View m_view;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // MTD_GRAPHICS_HPP
