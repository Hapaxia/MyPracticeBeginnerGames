/*

modifications:
add animated rotation to enemies
moved enemy "angle" calculation from Graphics to Enemies

*/

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
#include <SelbaWard/ConsoleScreen.hpp>

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
	kairos::Timestep timestep;
	sf::RenderWindow window;
	std::string currentStateString;
	bool stateHasChanged;
	Player player;
	Bullets bullets;
	Enemies enemies;
	pl::KeyMap keys;
	pl::ResourceManagerBasic resources;
	sw::ConsoleScreen cs;
	bool fireEventPressed;
	unsigned int score;

	void initKeys();
	bool initResources();
	bool initConsoleScreen();
	void reset();
	void update();
	void printScreen();
	void printScreenReady();
	void printScreenGameOver();
	void printScreenRunning();
	void printScreenPaused();
	unsigned int getColumnToCenterString(const std::string& string);
	bool blink(double hold, double duration);
};

#endif // MTD_GAME_HPP
