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
		window.setTitle(windowTitle + " || State: " + currentStateString);

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

	// graphics
	graphics.updateView(window.getView());
	graphics.updatePlayer(player);
	graphics.updateBullets(bullets);
	graphics.updateEnemies(enemies);
}
