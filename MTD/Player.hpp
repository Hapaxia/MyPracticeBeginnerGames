#ifndef MTD_PLAYER_HPP
#define MTD_PLAYER_HPP

#include <Plinth/Generic.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Graphics.hpp"

class Player
{
public:
	enum class Direction
	{
		Left,
		Right
	};

	Player(const sf::RenderWindow& window, double dt, const Graphics& graphics);
	void move(Direction direction);
	double getPosition() const;

private:
	double m_dt;
	double m_speed;
	pl::Range<double> m_positionLimits;
	double m_position;
};

#endif // MTD_PLAYER_HPP