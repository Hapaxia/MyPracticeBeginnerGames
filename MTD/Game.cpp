#include "Game.hpp"

Game::Game()
: windowTitle("MTD")
, timestep()
, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
, state(State::Ready)
{
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
				if (event.key.code == sf::Keyboard::Escape) // ends current game (or closes if state is currently "over")
				{
					if (state == State::Over)
					{
						window.close();
					}
					if (state != State::Ready)
					{
						state = State::Over;
					}
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

				// output current state to console whenever a key is pressed
				switch (state)
				{
				case State::Ready:
					DEV::printLine("State: Ready");
					break;
				case State::Paused:
					DEV::printLine("State: Paused");
					break;
				case State::Over:
					DEV::printLine("State: Over");
					break;
				case State::Running:
					DEV::printLine("State: Running");
					break;
				default:
					DEV::printLine("State: [unknown]");
					break;
				}
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired())
			update();

		// update display
		window.clear();
		//window.draw();
		window.display();
	}
}

void Game::reset()
{
}

void Game::update()
{
}
