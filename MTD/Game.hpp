/*

modifications:
starts in ready state
added score
added Console Screen from Selba Ward (https://github.com/Hapaxia/SelbaWard/wiki)
each state now shows different displays on the console screen
added ability to move through states with more intuitive controls (fire to start game from ready and to continue from game over, [P] to pause/resume)
timestep time is reset when state changes (for most changes) so that the time represents the amount of time in that state. it doesn't reset during pause - it should just pause

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
	const std::string windowTitle;
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
