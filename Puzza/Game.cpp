#include "Game.hpp"

Game::Game():
windowTitle("Puzza (WIP:2015) by Hapax (github.com/Hapaxia)"),
timestep(),
window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default),
scores(window),
ball(),
player(),
opponent(),
ballGraphic(ball.getRadius()),
playerGraphic({ 20.f, 100.f }),
opponentGraphic(playerGraphic.getSize()),
font(),
paddlePositionOffsetFromWindowSide(50.f),
paddleReachFromCenter(window.getSize().y * 0.4f)
{
	if (!font.loadFromFile("resources/arial.ttf"))
		throw "Failed to load font.";
	scores.setFont(font);
	window.setMouseCursorVisible(false);
	ballGraphic.setOrigin(ballGraphic.getRadius(), ballGraphic.getRadius());
	playerGraphic.setOrigin(sf::Vector2f{ playerGraphic.getLocalBounds().width, playerGraphic.getLocalBounds().height } / 2.f);
	opponentGraphic.setOrigin(sf::Vector2f{ opponentGraphic.getLocalBounds().width, opponentGraphic.getLocalBounds().height } / 2.f);
	ball.setPosition(sf::Vector2f(window.getSize() / 2u));
	opponent.setAcceleration(100.f);
	opponent.setDeceleration(250.f);
	opponent.setMaximumSpeed(2.f);
	timestep.setStep(1.0 / 500.0); // decrease timestep size to a 500th of a second to catch collisions at higher ball speeds
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
		window.setTitle(windowTitle +
			" | Ball Speed: " + pl::stringFrom(static_cast<int>(ball.getSpeed())) +
			" | Ball Spin: " + pl::stringFrom(ball.getSpin())
			);

		// update display
		window.clear();
		window.draw(ballGraphic);
		window.draw(playerGraphic);
		window.draw(opponentGraphic);
		window.draw(scores);
		window.display();
	}
}

void Game::resetBall()
{
	ball.setPosition(window.getView().getSize() / 2.f);
	ball.setDirection(115.f);
	ball.setSpeed(200.f);
	ball.setSpin(0.f);
}

void Game::update()
{
	updatePlayerPaddle();
	updateOpponentPaddle();
	updateBall();
	updateScores();
}

void Game::updatePlayerPaddle()
{
	// input
	const float mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;
	player.setTargetPosition((mouse / window.getView().getSize().y) * 2 - 1);

	// update
	player.update(timestep.getStepAsFloat());

	// update graphic
	playerGraphic.setPosition(sf::Vector2f{ paddlePositionOffsetFromWindowSide, (paddleReachFromCenter * player.getPosition()) + (window.getView().getSize().y / 2) });
}

void Game::updateOpponentPaddle()
{
	// input
	const float opponentTarget = ball.getPosition().y;
	opponent.setTargetPosition((opponentTarget - (window.getView().getSize().y / 2)) / paddleReachFromCenter);

	// update
	opponent.update(timestep.getStepAsFloat());

	// update graphic
	opponentGraphic.setPosition(sf::Vector2f{ window.getView().getSize().x - paddlePositionOffsetFromWindowSide, (paddleReachFromCenter * opponent.getPosition()) + (window.getView().getSize().y / 2) });
}

void Game::updateBall()
{
	// input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		ball.changeSpeed(3.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		ball.changeSpeed(-3.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		ball.setDirection(ball.getDirection() + 0.3f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		ball.setDirection(ball.getDirection() - 0.3f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
		ball.changeSpin(1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
		ball.changeSpin(-1.f);

	// update
	ball.update(timestep.getStepAsFloat());

	// collision with top or bottom
	if ((ball.getPosition().y < ball.getRadius()) || (ball.getPosition().y > window.getView().getSize().y - ball.getRadius()))
	{
		ball.setPosition({ ball.getPosition().x, pl::clamp(ball.getPosition().y, ball.getRadius(), window.getView().getSize().y - ball.getRadius()) });
		ball.flipDirectionVertically();
		ball.setSpin(ball.getSpin() * 0.5f);
	}

	// collision with sides
	if (ball.getPosition().x < ball.getRadius())
	{
		opponent.increaseScore();
		resetBall();
	}
	else if (ball.getPosition().x > window.getView().getSize().x - ball.getRadius())
	{
		player.increaseScore();
		resetBall();
	}

	// collision with player paddle
	if (ballGraphic.getGlobalBounds().intersects(playerGraphic.getGlobalBounds()))
	{
		if ((pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(playerGraphic).y, pl::Anchor::Global::getBottomCenter(playerGraphic).y })) &&
			(ball.getPosition().x > pl::Anchor::Global::getCenterRight(playerGraphic).x) &&
			(ball.getDirection() > 180.f))
		{
			ball.setPosition({ pl::Anchor::Global::getCenterRight(playerGraphic).x + ball.getRadius(), ball.getPosition().y });
			ball.flipDirectionHorizontally();
			ball.changeSpeed(25.f);
			ball.setSpin(ball.getSpin() * 0.5f);
			ball.changeSpin(-5.f * player.getSpeed() * ball.getSpeed() * timestep.getStepAsFloat());
		}
		else if (ball.getPosition().y < pl::Anchor::Global::getTopCenter(playerGraphic).y)
		{
			ball.setPosition({ ball.getPosition().x, pl::Anchor::Global::getTopCenter(playerGraphic).y - ball.getRadius() });
			if (pl::inRange(ball.getDirection(), pl::Range<float>{90.f, 270.f}))
				ball.flipDirectionVertically();
		}
		else if (ball.getPosition().y > pl::Anchor::Global::getBottomCenter(playerGraphic).y)
		{
			ball.setPosition({ ball.getPosition().x, pl::Anchor::Global::getBottomCenter(playerGraphic).y + ball.getRadius() });
			if (!pl::inRange(ball.getDirection(), pl::Range<float>{90.f, 270.f}))
				ball.flipDirectionVertically();
		}
	}

	// collision with opponent paddle
	if (ballGraphic.getGlobalBounds().intersects(opponentGraphic.getGlobalBounds()))
	{
		if ((pl::inRange(ball.getPosition().y, pl::Range<float>{ pl::Anchor::Global::getTopCenter(opponentGraphic).y, pl::Anchor::Global::getBottomCenter(opponentGraphic).y })) &&
			(ball.getPosition().x < pl::Anchor::Global::getCenterLeft(opponentGraphic).x) &&
			(ball.getDirection() < 180.f))
		{
			ball.setPosition({ pl::Anchor::Global::getCenterLeft(opponentGraphic).x - ball.getRadius(), ball.getPosition().y });
			ball.flipDirectionHorizontally();
			ball.changeSpeed(25.f);
			ball.setSpin(ball.getSpin() * 0.5f);
			ball.changeSpin(-5.f * opponent.getSpeed() * ball.getSpeed() * timestep.getStepAsFloat());
		}
		else if (ball.getPosition().y < pl::Anchor::Global::getTopCenter(opponentGraphic).y)
		{
			ball.setPosition({ ball.getPosition().x, pl::Anchor::Global::getTopCenter(opponentGraphic).y - ball.getRadius() });
			if (pl::inRange(ball.getDirection(), pl::Range<float>{90.f, 270.f}))
				ball.flipDirectionVertically();
		}
		else if (ball.getPosition().y > pl::Anchor::Global::getBottomCenter(opponentGraphic).y)
		{
			ball.setPosition({ ball.getPosition().x, pl::Anchor::Global::getBottomCenter(opponentGraphic).y + ball.getRadius() });
			if (!pl::inRange(ball.getDirection(), pl::Range<float>{90.f, 270.f}))
				ball.flipDirectionVertically();
		}
	}

	// update graphic
	ballGraphic.setPosition(ball.getPosition());
}

void Game::updateScores()
{
	scores.updatePlayer(player.getScore());
	scores.updateOpponent(opponent.getScore());
}
