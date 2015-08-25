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
	kairos::Timestep timestep;
	sf::RenderWindow window;
	Ball ball;
	Paddle paddle;
	sf::CircleShape ballGraphic;
	sf::RectangleShape paddleGraphic;
};

#endif // PUZZA_GAME_HPP
