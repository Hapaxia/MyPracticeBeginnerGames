#include "GameState_Win.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

namespace
{

const std::string winString{ "CONGRATULATIONS!" };
const std::string continueString{ "Press FIRE" };
const std::string finalScoreString{ "Final score" };

constexpr double winKeyPressDelay{ 1.0 }; // seconds

} // namespace

namespace GameState
{

Win::Win(Game& passedGame)
	: Base(passedGame)
{
	game.timestep.resetTime();
	game.cs.setShowBackground(false);
	printScreen();
}

std::unique_ptr<GameState::Base> Win::handleEvents()
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
			if ((event.key.code == game.keys.getKey("player shoot")) && (game.timestep.getTime() <= winKeyPressDelay))
				break;
			if ((event.key.code == game.keys.getKey("player shoot")) || (event.key.code == game.keys.getKey("quit")))
			{
				game.reset();
				return std::make_unique<GameState::Title>(game);
			}
			break;
		}
	}
	return nullptr;
}

std::unique_ptr<Base> Win::update()
{
	return nullptr;
}

void Win::draw()
{
	game.window.draw(game.graphics);
	game.window.draw(game.cs);
}

void Win::printScreen()
{
	game.cs.clear();

	game.cs << Cs::StretchType::Both << Cs::Location(getColumnToCenterString(game.cs, winString), game.cs.getMode().y / 2u - 1u) << winString << Cs::StretchType::None;
	if (game.timestep.getTime() > winKeyPressDelay)
		game.cs << Cs::Location(getColumnToCenterString(game.cs, continueString), 20) << continueString;

	const std::string finalScore{ finalScoreString + ": " + pl::stringFrom(game.score) };
	game.cs << Cs::Location(getColumnToCenterString(game.cs, finalScore), 8) << finalScore;
}

} // namespace GameState
