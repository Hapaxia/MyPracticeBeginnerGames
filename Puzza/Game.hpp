#ifndef PUZZA_GAME_HPP
#define PUZZA_GAME_HPP

#include <DevSfml.hpp>
#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>
#include "Ball.hpp"
#include "Paddle.hpp"

class Game
{
public:
	Game();
	void run();

public:
	Ball ball;
	kairos::Timestep timestep;
	sf::RenderWindow window;
	sf::CircleShape ballGraphic;
	Paddle paddle;
	sf::RectangleShape paddleGraphic;
};

#endif // PUZZA_GAME_HPP
