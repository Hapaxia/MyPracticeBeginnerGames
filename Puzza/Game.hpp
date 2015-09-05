#ifndef PUZZA_GAME_HPP
#define PUZZA_GAME_HPP

#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Scores.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Resources.hpp"
#include "Graphics.hpp"
#include "Music.hpp"

class Game
{
public:
	Game();
	void run();

private:
	enum class State
	{
		Ready,
		Running,
		Paused,
		Over
	} state = State::Ready;

	Resources resources;
	Graphics graphics;
	Music music;
	const std::string windowTitle;
	kairos::Timestep timestep;
	sf::RenderWindow window;
	Scores scores;
	Ball ball;
	Paddle player;
	Paddle opponent;
	sf::Sound sound;
	sf::RectangleShape background;
	sf::Text message;
	sf::RectangleShape messageBox;
	bool isMuted;
	sf::Text url;

	void reset();
	void resetBall();
	void update();
	void updatePlayerPaddle();
	void updateOpponentPaddle();
	void updateBall();
	void updateScores();
	void playSound(const std::string& soundName);
};

#endif // PUZZA_GAME_HPP
