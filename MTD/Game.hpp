/*

modifications:
separated game over states for win and lose

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
	pl::ResourceManagerBasic resources;
	sf::RenderWindow window;
	Player player;
	Bullets bullets;
	Enemies enemies;
	pl::KeyMap keys;
	sw::ConsoleScreen cs;
	bool allowPlayerBulletFire;
	unsigned int highScore;
	unsigned int score;
	unsigned int lives;
	Graphics graphics;
	kairos::Timestep timestep;

	Game();

	bool isRunning() const { return isGameRunning; }
	void quit() { isGameRunning = false; }

	void handleEvents();
	void update();
	void draw();

	void reset();

private:
	bool isGameRunning;

	void initKeys();
	bool initResources();
	bool initConsoleScreen();
};

#endif // MTD_GAME_HPP
