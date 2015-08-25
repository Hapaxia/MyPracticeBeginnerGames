#include "Game.hpp"

Game::Game():
windowTitle("Puzza (WIP:2015) by Hapax (github.com/Hapaxia)"),
timestep(),
window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default),
ball(),
player(),
opponent(),
ballGraphic(ball.getRadius()),
playerGraphic({ 20.f, 100.f }),
opponentGraphic(playerGraphic.getSize()),
paddlePositionOffsetFromWindowSide(50.f),
paddleReachFromCenter(window.getSize().y * 0.4f)
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
				if (event.key.code == sf::Keyboard::F12) // forces program error
					throw "Forced exception";
				else if (event.key.code == sf::Keyboard::F1) // stops the ball
					ball.setSpeed(0.f);
				else if (event.key.code == sf::Keyboard::F2) // resets and restarts the ball in the default position, direction and speed
					resetBall();
			}
		}

		// main updates
		timestep.addFrame();
		while (timestep.isUpdateRequired())
			update();

		// update window title
		window.setTitle(windowTitle + " | Ball Speed: " + pl::stringFrom(static_cast<int>(ball.getSpeed())));

		// update display
		window.clear();
		window.draw(ballGraphic);
		window.draw(playerGraphic);
		window.draw(opponentGraphic);
		window.display();
	}
}

void Game::resetBall()
{
	ball.setPosition(sf::Vector2f(window.getSize() / 2u));
	ball.setDirection(115.f);
	ball.setSpeed(200.f);
}

void Game::update()
{
	updatePlayerPaddle();
	updateOpponentPaddle();
	updateBall();
}

void Game::updatePlayerPaddle()
{
	// input
	const float mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
	player.setTargetPosition((mouse / window.getSize().y) * 2 - 1);

	// update
	player.update(timestep.getStepAsFloat());

	// update graphic
	playerGraphic.setPosition(sf::Vector2f{ paddlePositionOffsetFromWindowSide, (paddleReachFromCenter * player.getPosition()) + (window.getSize().y / 2) });
}

void Game::updateOpponentPaddle()
{
	// input
	const float opponentTarget = ball.getPosition().y;
	opponent.setTargetPosition((opponentTarget - (window.getSize().y / 2)) / paddleReachFromCenter);

	// update
	opponent.update(timestep.getStepAsFloat());

	// update graphic
	opponentGraphic.setPosition(sf::Vector2f{ window.getSize().x - paddlePositionOffsetFromWindowSide, (paddleReachFromCenter * opponent.getPosition()) + (window.getSize().y / 2) });
}

void Game::updateBall()
{
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

	// collision with top or bottom
	if ((ball.getPosition().y < ball.getRadius()) || (ball.getPosition().y > window.getSize().y - ball.getRadius()))
	{
		ball.setPosition({ ball.getPosition().x, pl::clamp(ball.getPosition().y, ball.getRadius(), window.getSize().y - ball.getRadius()) });
		ball.flipDirectionVertically();
	}

	// collision with sides
	if (ball.getPosition().x < ball.getRadius())
		resetBall();
	else if (ball.getPosition().x > window.getSize().x - ball.getRadius())
		resetBall();

	// collision with player paddle
	if (ballGraphic.getGlobalBounds().intersects(playerGraphic.getGlobalBounds()) &&
		(pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(playerGraphic).y, pl::Anchor::Global::getBottomCenter(playerGraphic).y })) &&
		(ball.getPosition().x > pl::Anchor::Global::getCenterRight(playerGraphic).x) &&
		(ball.getDirection() > 180.f))
	{
		ball.setPosition({ pl::Anchor::Global::getCenterRight(playerGraphic).x + ball.getRadius(), ball.getPosition().y });
		ball.flipDirectionHorizontally();
		ball.changeSpeed(25.f);
	}

	// collision with opponent paddle
	if (ballGraphic.getGlobalBounds().intersects(opponentGraphic.getGlobalBounds()) &&
		(pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(opponentGraphic).y, pl::Anchor::Global::getBottomCenter(opponentGraphic).y })) &&
		(ball.getPosition().x < pl::Anchor::Global::getCenterLeft(opponentGraphic).x) &&
		(ball.getDirection() < 180.f))
	{
		ball.setPosition({ pl::Anchor::Global::getCenterLeft(opponentGraphic).x - ball.getRadius(), ball.getPosition().y });
		ball.flipDirectionHorizontally();
		ball.changeSpeed(25.f);
	}

	// update graphic
	ballGraphic.setPosition(ball.getPosition());
}
