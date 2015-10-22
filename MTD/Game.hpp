#ifndef MTD_GAME_HPP
#define MTD_GAME_HPP

#include <Dev.hpp>
#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>
#include "Graphics.hpp"
#include "Player.hpp"
#include "Bullets.hpp"
#include "Enemies.hpp"
#include <Plinth/Sfml/KeyMap.hpp>
#include <vector>

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

	Graphics graphics;
	const std::string windowTitle;
	kairos::Timestep timestep;
	sf::RenderWindow window;
	std::string currentStateString;
	bool doesStateStringNeedUpdating;
	Player player;
	Bullets bullets;
	Enemies enemies;
	pl::KeyMap keys;

	void reset();
	void update();
};

#endif // MTD_GAME_HPP
