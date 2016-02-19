#include "Game.hpp"

namespace
{

template <class T, class U>
void addElementToVectorIfUnique(std::vector<T>& vector, const U& elementToAdd)
{
	auto it = std::find(vector.begin(), vector.end(), static_cast<T>(elementToAdd));
	if (it == vector.end())
		vector.emplace_back(elementToAdd);
}

} // namespace

Game::Game()
	: windowTitle("MTD")
	, timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	, state(State::Running)
	, currentStateString("Running")
	, doesStateStringNeedUpdating(false)
	//, state(State::Ready)
	//, currentStateString("Ready")
	, graphics()
	, player(window, timestep.getStep(), graphics)
	, bullets()
	, enemies(window)
	, keys()
{
	keys.addKey(sf::Keyboard::Left,   "player left");
	keys.addKey(sf::Keyboard::Right,  "player right");
	keys.addKey(sf::Keyboard::Space,  "player shoot");
	keys.addKey(sf::Keyboard::Tab,    "progress state");
	keys.addKey(sf::Keyboard::Escape, "quit");
	keys.addKey(sf::Keyboard::Return, "toggle enemy direction");
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
						doesStateStringNeedUpdating = true;
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
					doesStateStringNeedUpdating = true;
				}
				else if (event.key.code == keys.getKey("toggle enemy direction")) // toggle enemies' horizontal direction
					enemies.toggleDirection();
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired() && state == State::Running)
			update();

		// update state string (if it has changed)
		if (doesStateStringNeedUpdating)
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
			doesStateStringNeedUpdating = false;
		}

		// update window title
		window.setTitle(windowTitle + " || State: " + currentStateString +
			" || Enemies: " + pl::stringFrom(enemies.getNumberOfEnemiesAlive()) +
			" || Enemy Speed: " + pl::stringFrom(enemies.getSpeed()) +
			" || Enemy Drop Speed: " + pl::stringFrom(enemies.getDropSpeed()));

		// update display
		window.clear();
		window.draw(graphics);
		window.display();
	}
}

void Game::reset()
{
	enemies.reset();
	bullets.reset();
	graphics.clearEnemies();
	graphics.clearBullets();
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
		enemies.killEnemy(enemyToRemove);
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
		doesStateStringNeedUpdating = true;
	}
	if (progression == Progression::EnemiesWon)
	{
		state = State::Over;
		doesStateStringNeedUpdating = true;
	}
}
