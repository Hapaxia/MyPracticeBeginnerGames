#include "GameState_Death.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

#include <cmath>

namespace
{

constexpr unsigned int deathTimeInSeconds{ 3u };

const std::string pausedString{ "DIED!" };
const std::string continueStringPt1{ "Will continue after " };
const std::string continueStringPt2{ " seconds" };

const sf::Time deathTime{ sf::seconds(static_cast<float>(deathTimeInSeconds)) };

} // namespace

namespace GameState
{

Death::Death(Game& passedGame)
	: Base(passedGame)
	, m_clock()
{
	game.timestep.pause();
	game.cs.setShowBackground(false);
	printScreen();

	m_clock.restart();
}

std::unique_ptr<GameState::Base> Death::handleEvents()
{
	sf::Event event;
	while (game.window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			game.quit();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == game.keys.getKey("quit"))
				return std::make_unique<GameState::Over>(game);
			break;
		}
	}
	return nullptr;
}

std::unique_ptr<Base> Death::update()
{
	for (auto& enemy : game.enemies)
	{
		enemy.updateBullet(game.timestep.getStep());
	}
	game.graphics.updateEnemies(game.enemies);

	if (m_clock.getElapsedTime() > deathTime)
		return std::make_unique<GameState::Play>(game);

	return nullptr;
}

void Death::draw()
{
	game.window.draw(game.graphics);
	game.window.draw(game.cs);
}

void Death::printScreen()
{
	game.cs.clear();

	const std::string continueString{ continueStringPt1 + pl::stringFrom(static_cast<unsigned int>(std::ceil((deathTime - m_clock.getElapsedTime()).asSeconds()) + 0.5f)) + continueStringPt2 };

	game.cs << Cs::StretchType::Both << Cs::Location(getColumnToCenterString(game.cs, pausedString), game.cs.getMode().y / 2u - 1u) << pausedString << Cs::StretchType::None;
	game.cs << Cs::Location(getColumnToCenterString(game.cs, continueString), 20) << continueString;
}

} // namespace GameState
