#include "Game.hpp"

namespace
{

const std::vector<std::string> infoTitle
{
	{ "M" },
	{ " T" },
	{ "  D " }
};
const std::vector<std::string> infoKeys
{
	{ " left \x10" },
	{ "right \x11" },
	{ " fire SPACE" },
	{ " quit ESC" },
	{ "pause P" }
};

template <class T, class U>
void addElementToVectorIfUnique(std::vector<T>& vector, const U& elementToAdd)
{
	const T element{ static_cast<T>(elementToAdd) };
	if (std::find(vector.begin(), vector.end(), element) == vector.end())
		vector.emplace_back(element);
}

} // namespace

Game::Game()
	: windowTitle("MTD")
	, timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	//, state(State::Running)
	//, currentStateString("Running")
	, stateHasChanged(false)
	, state(State::Ready)
	, currentStateString("Ready")
	, graphics()
	, player(window, timestep.getStep(), graphics)
	, bullets()
	, enemies(window)
	, keys()
	, resources()
	, cs()
	, fireEventPressed(false)
	, score(0u)
{
	initKeys();
	if (!initResources())
		throw "Failed to initialise resources.";
	if (!initConsoleScreen())
		throw "Failed to initialise Console Screen.";
	window.setMouseCursorVisible(false);
}

void Game::run()
{
	while (window.isOpen())
	{
		// process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == keys.getKey("quit")) // ends current game (or closes if state is currently "over" or "ready")
				{
					if (state == State::Over || state == State::Ready)
						window.close();
					else
					{
						state = State::Over;
						stateHasChanged = true;
						timestep.resetTime();
					}
				}
				else if (event.key.code == keys.getKey("progress state")) // progresses state
				{
					switch (state)
					{
					case State::Ready:
					case State::Paused:
						state = State::Running;
						break;
					case State::Over:
						reset();
						state = State::Ready;
						break;
					case State::Running:
					default:
						state = State::Paused;
						break;
					}
					stateHasChanged = true;
				}
				else if (event.key.code == keys.getKey("toggle enemy direction")) // toggle enemies' horizontal direction
					enemies.toggleDirection();
				else if (event.key.code == keys.getKey("player shoot"))
				{
					if (state != State::Running)
						fireEventPressed = true;
				}
				else if (event.key.code == keys.getKey("pause"))
				{
					if (state == State::Paused)
					{
						state = State::Running;
						stateHasChanged = true;
						timestep.unpause();
					}
					else if (state == State::Running)
					{
						timestep.pause();
						state = State::Paused;
						stateHasChanged = true;
					}
				}
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == keys.getKey("player shoot") && fireEventPressed)
				{
					if (state == State::Ready)
					{
						state = State::Running;
						stateHasChanged = true;
						timestep.resetTime();
					}
					else if (state == State::Over && timestep.getTime() > 1.0) // delay ability to skip game over screen to avoid skipping by accident
					{
						reset();
						state = State::Ready;
						stateHasChanged = true;
					}
					fireEventPressed = false;
				}
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired() && state == State::Running)
			update();

		// update console screen
		printScreen();

		// update state string (if it has changed)
		if (stateHasChanged)
		{
			switch (state)
			{
			case State::Ready:
				currentStateString = "Ready";
				break;
			case State::Paused:
				currentStateString = "Paused";
				break;
			case State::Over:
				currentStateString = "Over";
				break;
			case State::Running:
				currentStateString = "Running";
				break;
			default:
				currentStateString = "[unknown]";
			}
			stateHasChanged = false;
		}

		// update window title
		window.setTitle(windowTitle + " || State: " + currentStateString +
			" || Enemies: " + pl::stringFrom(enemies.getNumberOfEnemiesAlive()) +
			" || Enemy Speed: " + pl::stringFrom(enemies.getSpeed()) +
			" || Enemy Drop Speed: " + pl::stringFrom(enemies.getDropSpeed()));

		// update display
		window.clear();
		if (state != State::Ready)
			window.draw(graphics);
		window.draw(cs);
		window.display();
	}
}

void Game::initKeys()
{
	keys.addKey(sf::Keyboard::Left, "player left");
	keys.addKey(sf::Keyboard::Right, "player right");
	keys.addKey(sf::Keyboard::Space, "player shoot");
	keys.addKey(sf::Keyboard::P, "pause");
	keys.addKey(sf::Keyboard::Tab, "progress state");
	keys.addKey(sf::Keyboard::Escape, "quit");
	keys.addKey(sf::Keyboard::Return, "toggle enemy direction");
}

bool Game::initResources()
{
	try
	{
		resources.addTexture("font sheet", "resources/fontsheet.png");
	}
	catch (pl::Exception&)
	{
		return false;
	}
	return true;
}

bool Game::initConsoleScreen()
{
	const sf::Vector2u textureTileSize{ 8u, 8u };
	const sf::Vector2u consoleScreenScale{ 3u, 3u };
	cs.setTexture(resources.getTexture("font sheet"));
	cs.setTextureTileSize(textureTileSize);
	cs.setNumberOfTextureTilesPerRow(16);
	cs.setMode({ 33, 25 });
	cs.setSize({ static_cast<float>(cs.getMode().x * textureTileSize.x * consoleScreenScale.x), static_cast<float>(cs.getMode().y * textureTileSize.y * consoleScreenScale.y) });
	cs.setPosition((window.getSize().x - cs.getSize().x) / 2, 0);
	cs.setShowCursor(false);
	cs.setShowBackground(false);
	cs.crash();

	return true;
}

void Game::reset()
{
	player.reset();
	enemies.reset();
	bullets.reset();
	graphics.clearEnemies();
	graphics.clearBullets();
	score = 0;
}

void Game::update()
{
	// input
	int controlDirection = 0;
	if (sf::Keyboard::isKeyPressed(keys.getKey("player left")))
		--controlDirection;
	if (sf::Keyboard::isKeyPressed(keys.getKey("player right")))
		++controlDirection;
	if (sf::Keyboard::isKeyPressed(keys.getKey("player shoot")))
		bullets.shoot({ player.getPosition(), window.getView().getSize().y });

	// update
	if (controlDirection != 0)
	{
		if (controlDirection > 0)
			player.move(Player::Direction::Right);
		else
			player.move(Player::Direction::Left);
	}
	bullets.update(timestep.getStep());
	enemies.update(timestep.getStep());

	enum class Progression
	{
		None,
		EnemiesWon,
		EnemiesDestroyed
	} progression = Progression::None;

	// prepare player's bounding box
	pl::Vector2d playerHalfSize{ player.getSize().x / 2.0, player.getSize().y };
	pl::RangeArea<double> playerBoundingBox;
	playerBoundingBox.setLeftBottom(pl::Vector2d{ player.getPosition(), window.getView().getSize().y } - playerHalfSize);
	playerBoundingBox.setRightTop({ player.getPosition() + playerHalfSize.x, window.getView().getSize().y });

	// remove enemies hit by bullets
	pl::RangeArea<double> enemyBoundingBox;
	pl::RangeArea<double> bulletBoundingBox;
	pl::Vector2d enemyHalfSize{ 0.0, 0.0 };
	pl::Vector2d bulletHalfSize{ 0.0, 0.0 };
	std::vector<unsigned int> enemiesToRemove;
	std::vector<unsigned int> bulletsToRemove;
	for (auto enemy = enemies.begin(), enemiesEnd = enemies.end(); enemy != enemiesEnd; ++enemy)
	{
		if (!enemy->isAlive())
			continue;
		enemyHalfSize = enemy->getSize() / 2.0;
		enemyBoundingBox.setLeftBottom(enemy->getPosition() - enemyHalfSize);
		enemyBoundingBox.setRightTop(enemy->getPosition() + enemyHalfSize);
		if (enemyBoundingBox.overlaps(playerBoundingBox))
		{
			progression = Progression::EnemiesWon;
			break;
		}
		unsigned int bulletNumber{ 0u };
		for (auto& bullet : bullets)
		{
			if (bullet.isAlive())
			{
				bulletHalfSize = bullet.getSize() / 2.0;
				bulletBoundingBox.setLeftBottom(bullet.getPosition() - bulletHalfSize);
				bulletBoundingBox.setRightTop(bullet.getPosition() + bulletHalfSize);
				if (enemyBoundingBox.overlaps(bulletBoundingBox))
				{
					addElementToVectorIfUnique(enemiesToRemove, enemy - enemies.begin());
					addElementToVectorIfUnique(bulletsToRemove, bulletNumber);
				}
			}
			++bulletNumber;
		}
	}
	for (auto& enemyToRemove : enemiesToRemove)
	{
		enemies.killEnemy(enemyToRemove);
		score += 10;
		score += static_cast<unsigned int>(enemies.getDropSpeed() * 100 + enemies.getSpeed() / 50);
	}
	for (auto& bulletToRemove : bulletsToRemove)
		bullets.killBullet(bulletToRemove);
	if (enemies.getNumberOfEnemiesAlive() == 0)
		progression = Progression::EnemiesDestroyed;
	if (enemies.getReachedBottom())
		progression = Progression::EnemiesWon;

	// graphics
	graphics.updateView(window.getView());
	graphics.updatePlayer(player);
	graphics.updateBullets(bullets);
	graphics.updateEnemies(enemies);

	// update state
	if (progression == Progression::EnemiesDestroyed)
	{
		state = State::Over;
		stateHasChanged = true;
		timestep.resetTime();
	}
	if (progression == Progression::EnemiesWon)
	{
		state = State::Over;
		stateHasChanged = true;
		timestep.resetTime();
	}
}

inline void Game::printScreen()
{
	switch (state)
	{
	case State::Ready:
		printScreenReady();
		break;
	case State::Paused:
		printScreenPaused();
		break;
	case State::Over:
		printScreenGameOver();
		break;
	case State::Running:
		printScreenRunning();
		break;
	default:
		cs.clear();
	}
}

void Game::printScreenReady()
{
	cs.clear();
	unsigned int row{ 2u };
	for (auto& line : infoTitle)
	{
		cs.printStretchedAt({ getColumnToCenterString(infoTitle.back()), row++ }, line);
		++row;
	}
	row = 11u;
	for (auto& line : infoKeys)
		cs.printAt({ 10, row++ }, line);

	const std::string startString{ "Press FIRE to start" };
	if (blink(1.0, 1.5))
		cs.printAt({ getColumnToCenterString(startString), 20 }, startString);
}

void Game::printScreenGameOver()
{
	cs.clear();
	const std::string gameOverString{ "GAME OVER!" };
	const std::string continueString{ "Press FIRE" };
	cs.printStretchedAt({ getColumnToCenterString(gameOverString), cs.getMode().y / 2u - 1u }, gameOverString);
	cs.printAt({ getColumnToCenterString(continueString), 20 }, continueString);
}

void Game::printScreenRunning()
{
	cs.clear();
	cs.printAt({ 0, 0 }, "SCORE " + pl::padStringLeft(pl::stringFrom(score), 4, '0'));
	cs.printAt({ cs.getMode().x - 9, 0 }, "7590 HIGH");
}

void Game::printScreenPaused()
{
	cs.clear();
	const std::string pausedString{ "PAUSED!" };
	const std::string resumeString{ "Press P to resume" };
	cs.printStretchedAt({ getColumnToCenterString(pausedString), cs.getMode().y / 2u - 1u }, pausedString);
	cs.printAt({ getColumnToCenterString(resumeString), 20 }, resumeString);
}

inline unsigned int Game::getColumnToCenterString(const std::string& string)
{
	return (cs.getMode().x - string.size()) / 2u;
}

inline bool Game::blink(double hold, double duration)
{
	return pl::mod(timestep.getTime(), duration) < hold;
}
