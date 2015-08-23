#include "Game.hpp"

Game::Game():
ball(),
timestep(),
window(sf::VideoMode(800, 600), "Puzza", sf::Style::Default),
ballGraphic(ball.getRadius()),
paddle(),
paddleGraphic({ 20.f, 100.f })
{
	ballGraphic.setOrigin(ballGraphic.getRadius(), ballGraphic.getRadius());
	ball.setPosition(sf::Vector2f(window.getSize() / 2u));
	paddleGraphic.setOrigin(sf::Vector2f{ paddleGraphic.getLocalBounds().width, paddleGraphic.getLocalBounds().height } / 2.f);
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
			// update paddle

			// input
			const float mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
			paddle.setTargetPosition((mouse / window.getSize().y) * 2 - 1);

			// update
			paddle.update(timestep.getStepAsFloat());



			// update ball

			// input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				ball.changeSpeed(5.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				ball.changeSpeed(-5.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				ball.setDirection(ball.getDirection() + 1.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				ball.setDirection(ball.getDirection() - 1.f);

			// update
			ball.update(timestep.getStepAsFloat());
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
		paddleGraphic.setPosition(sf::Vector2f{ 30.f, ((window.getSize().y * 0.4f) * paddle.getPosition()) + (window.getSize().y / 2) });

		// update display
		window.clear();
		window.draw(ballGraphic);
		window.draw(paddleGraphic);
		window.display();
	}
}
