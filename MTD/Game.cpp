#include "Game.hpp"

Game::Game()
	: windowTitle("MTD")
	, timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	, state(State::Ready)
	, currentStateString("Ready")
	, graphics()
	, player(window, timestep.getStep(), graphics)
	, keys()
{
	keys.addKey("player left", sf::Keyboard::A);
	keys.addKey("player right", sf::Keyboard::D);
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
				if (event.key.code == sf::Keyboard::Escape) // ends current game (or closes if state is currently "over" or "ready")
				{
					if (state == State::Over || state == State::Ready)
						window.close();
					else
						state = State::Over;
				}
				else if (event.key.code == sf::Keyboard::Space) // progresses state
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
		while (timestep.isUpdateRequired())
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

	// update
	if (controlDirection != 0)
	{
		if (controlDirection > 0)
			player.move(Player::Direction::Right);
		else
			player.move(Player::Direction::Left);
	}

	// graphics
	graphics.updatePlayer(window.getView(), player);
}
