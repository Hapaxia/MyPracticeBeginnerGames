#include "Game.hpp"

Game::Game():
timestep(),
window(sf::VideoMode(800, 600), "Puzza", sf::Style::Default),
ball(),
player(),
opponent(),
ballGraphic(ball.getRadius()),
playerGraphic({ 20.f, 100.f }),
opponentGraphic(playerGraphic.getSize()),
paddlePositionOffsetFromWindowSide(50.f)
{
	ballGraphic.setOrigin(ballGraphic.getRadius(), ballGraphic.getRadius());
	playerGraphic.setOrigin(sf::Vector2f{ playerGraphic.getLocalBounds().width, playerGraphic.getLocalBounds().height } / 2.f);
	opponentGraphic.setOrigin(sf::Vector2f{ opponentGraphic.getLocalBounds().width, opponentGraphic.getLocalBounds().height } / 2.f);
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
			// update player paddle

			// input
			const float mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
			player.setTargetPosition((mouse / window.getSize().y) * 2 - 1);

			// update
			player.update(timestep.getStepAsFloat());

			// update graphic
			playerGraphic.setPosition(sf::Vector2f{ paddlePositionOffsetFromWindowSide, ((window.getSize().y * 0.4f) * player.getPosition()) + (window.getSize().y / 2) });



			// update opponent paddle

			// input
			const float opponentTarget = ((ball.getPosition().y - (window.getSize().y / 2)) / 0.8f) + (window.getSize().y / 2);
			opponent.setTargetPosition((opponentTarget / window.getSize().y) * 2 - 1);

			// update
			opponent.update(timestep.getStepAsFloat());

			// update graphic
			opponentGraphic.setPosition(sf::Vector2f{ window.getSize().x - paddlePositionOffsetFromWindowSide, ((window.getSize().y * 0.4f) * opponent.getPosition()) + (window.getSize().y / 2) });



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

			// update graphic
			ballGraphic.setPosition(ball.getPosition());

			// collision with borders
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

			// collision with player paddle
			if (ballGraphic.getGlobalBounds().intersects(playerGraphic.getGlobalBounds()) &&
				(pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(playerGraphic).y, pl::Anchor::Global::getBottomCenter(playerGraphic).y })) &&
				(ball.getPosition().x > pl::Anchor::Global::getCenterRight(playerGraphic).x) &&
				(ball.getDirection() > 180.f))
			{
				ball.setPosition({ pl::Anchor::Global::getCenterRight(playerGraphic).x + ball.getRadius(), ball.getPosition().y });
				ball.flipDirectionHorizontally();
			}

			// collision with opponent paddle
			if (ballGraphic.getGlobalBounds().intersects(opponentGraphic.getGlobalBounds()) &&
				(pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(opponentGraphic).y, pl::Anchor::Global::getBottomCenter(opponentGraphic).y })) &&
				(ball.getPosition().x < pl::Anchor::Global::getCenterLeft(opponentGraphic).x) &&
				(ball.getDirection() < 180.f))
			{
				ball.setPosition({ pl::Anchor::Global::getCenterLeft(opponentGraphic).x - ball.getRadius(), ball.getPosition().y });
				ball.flipDirectionHorizontally();
			}
		}



		// update display
		window.clear();
		window.draw(ballGraphic);
		window.draw(playerGraphic);
		window.draw(opponentGraphic);
		window.display();
	}
}
