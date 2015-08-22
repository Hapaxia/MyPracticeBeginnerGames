#include "Game.hpp"

Game::Game():
ball(),
timestep(),
window(sf::VideoMode(800, 600), "Puzza", sf::Style::Default),
ballGraphic(ball.getRadius())
{
	ballGraphic.setOrigin(ballGraphic.getRadius(), ballGraphic.getRadius());
	ball.setPosition(sf::Vector2f(window.getSize() / 2u));
}

void Game::run()
{
	while (window.isOpen())
	{
		// process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (DEV::eventIsDefaultCloseWindow(event))
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::F12)
					throw "Forced exception";
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				ball.changeSpeed(0.1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				ball.changeSpeed(-0.1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				ball.setDirection(ball.getDirection() + 1.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				ball.setDirection(ball.getDirection() - 1.f);

			ball.moveForwards();
			if (ball.getPosition().x < ball.getRadius())
			{
				ball.setPosition({ ball.getRadius(), ball.getPosition().y });
				ball.flipDirectionHorizontally();
			}
			else if (ball.getPosition().x > window.getSize().x - ball.getRadius())
			{
				ball.setPosition({ window.getSize().x - ball.getRadius(), ball.getPosition().y });
				ball.flipDirectionHorizontally();
			}
			if (ball.getPosition().y < ball.getRadius())
			{
				ball.setPosition({ ball.getPosition().x, ball.getRadius() });
				ball.flipDirectionVertically();
			}
			else if (ball.getPosition().y > window.getSize().y - ball.getRadius())
			{
				ball.setPosition({ ball.getPosition().x, window.getSize().y - ball.getRadius() });
				ball.flipDirectionVertically();
			}
		}

		// update graphics
		ballGraphic.setPosition(ball.getPosition());

		// update display
		window.clear();
		window.draw(ballGraphic);
		window.display();
	}
}
