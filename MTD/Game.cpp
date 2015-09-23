#include "Game.hpp"

Game::Game()
	: windowTitle("MTD")
	, timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	, state(State::Ready)
	, currentStateString("State: Ready")
	, graphics()
	, player(window, timestep.getStep(), graphics)
	, bullets()
	, keys()
{
	/*
	keys.addKey("player left",    sf::Keyboard::Left);
	keys.addKey("player right",   sf::Keyboard::Right);
	keys.addKey("player shoot",   sf::Keyboard::Space);
	keys.addKey("progress state", sf::Keyboard::Tab);
	keys.addKey("quit",           sf::Keyboard::Escape);
	*/
	keys.addKey(sf::Keyboard::Left,   "player left");
	keys.addKey(sf::Keyboard::Right,  "player right");
	keys.addKey(sf::Keyboard::Space,  "player shoot");
	keys.addKey(sf::Keyboard::Tab,    "progress state");
	keys.addKey(sf::Keyboard::Escape, "quit");
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

				// update current state string whenever a key is pressed (currently, state can only change by pressing keys)
				switch (state)
				{
				case State::Ready:
					currentStateString = "State: Ready";
					break;
				case State::Paused:
					currentStateString = "State: Paused";
					break;
				case State::Over:
					currentStateString = "State: Over";
					break;
				case State::Running:
					currentStateString = "State: Running";
					break;
				default:
					currentStateString = "State: [unknown]";
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

	// graphics
	graphics.updatePlayer(window.getView(), player);
	graphics.updateBullets(window.getView(), bullets);
}
