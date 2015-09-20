#ifndef MTD_GAME_HPP
#define MTD_GAME_HPP

#include <Dev.hpp>
#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>

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
	} state;

	const std::string windowTitle;
	kairos::Timestep timestep;
	sf::RenderWindow window;

	void reset();
	void update();
};

#endif // MTD_GAME_HPP
