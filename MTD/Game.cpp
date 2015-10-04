#include "Game.hpp"

Game::Game()
	: windowTitle("MTD")
	, timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	, state(State::Running)
	, currentStateString("Running")
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
						state = State::Over;
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
				}
				else if (event.key.code == keys.getKey("toggle enemy direction")) // toggle enemies' horizontal direction
					enemies.toggleDirection();

				// update current state string whenever a key is pressed (currently, state can only change by pressing keys)
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
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired() && state == State::Running)
			update();

		// update window title
		window.setTitle(windowTitle + " || State: " + currentStateString +
			" || Enemies: " + pl::stringFrom(enemies.getNumberOfEnemiesAlive()) +
			" || Enemy Speed: " + pl::stringFrom(enemies.getSpeed()));

		// update display
		window.clear();
		window.draw(graphics);
		window.display();
	}
}

void Game::reset()
{
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

	// remove enemies hit by bullets
	pl::AreaRange<double> bulletBoundingBox;
	pl::AreaRange<double> enemyBoundingBox;
	pl::Vector2d enemyCenter{ 0.0, 0.0 };
	pl::Vector2d bulletCenter{ 0.0, 0.0 };
	std::vector<unsigned int> enemiesToRemove;
	std::vector<unsigned int> bulletsToRemove;
	for (auto enemy = enemies.begin(), enemiesEnd = enemies.end(); enemy != enemiesEnd; ++enemy)
	{
		if (!enemy->isAlive())
			continue;
		enemyCenter = enemy->getSize() / 2.0;
		enemyBoundingBox.setLeftBottom(enemy->getPosition() - enemyCenter);
		enemyBoundingBox.setRightTop(enemy->getPosition() + enemyCenter);
		unsigned int bulletNumber{ 0u };
		for (auto& bullet : bullets)
		{
			if (!bullet.isAlive())
				continue;
			bulletCenter = bullet.getSize() / 2.0;
			bulletBoundingBox.setLeftBottom(bullet.getPosition() - bulletCenter);
			bulletBoundingBox.setRightTop(bullet.getPosition() + bulletCenter);
			if (pl::areOverlapping(enemyBoundingBox, bulletBoundingBox))
			{
				auto findEnemyIt = std::find(enemiesToRemove.begin(), enemiesToRemove.end(), enemy - enemies.begin());
				if (findEnemyIt == enemiesToRemove.end())
					enemiesToRemove.emplace_back(enemy - enemies.begin());
				auto findBulletIt = std::find(bulletsToRemove.begin(), bulletsToRemove.end(), bulletNumber);
				if (findBulletIt == bulletsToRemove.end())
					bulletsToRemove.emplace_back(bulletNumber);
			}
			++bulletNumber;
		}
	}
	for (auto& enemyToRemove : enemiesToRemove)
		enemies.killEnemy(enemyToRemove);
	for (auto& bulletToRemove : bulletsToRemove)
		bullets.killBullet(bulletToRemove);



	// graphics
	graphics.updateView(window.getView());
	graphics.updatePlayer(player);
	graphics.updateBullets(bullets);
	graphics.updateEnemies(enemies);
}
