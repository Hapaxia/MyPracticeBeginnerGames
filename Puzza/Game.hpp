#ifndef PUZZA_GAME_HPP
#define PUZZA_GAME_HPP

#include <DevSfml.hpp>
#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>
#include "Scores.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

class Game
{
public:
	Game();
	void run();

private:
	const std::string windowTitle;
	kairos::Timestep timestep;
	sf::RenderWindow window;
	Scores scores;
	Ball ball;
	Paddle player;
	Paddle opponent;
	sf::CircleShape ballGraphic;
	sf::RectangleShape playerGraphic;
	sf::RectangleShape opponentGraphic;
	sf::Font font;
	const float paddlePositionOffsetFromWindowSide;
	const float paddleReachFromCenter;

	void reset();
	void resetBall();
	void update();
	void updatePlayerPaddle();
	void updateOpponentPaddle();
	void updateBall();
	void updateScores();
};

#endif // PUZZA_GAME_HPP
