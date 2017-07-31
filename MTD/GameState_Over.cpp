#include "GameState_Over.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

namespace
{

const std::string gameOverString{ "GAME OVER!" };
const std::string continueString{ "Press FIRE" };

constexpr double gameOverKeyPressDelay{ 1.0 }; // seconds

} // namespace

namespace GameState
{

Over::Over(Game& passedGame)
	: Base(passedGame)
{
	game.timestep.resetTime();
	game.cs.setShowBackground(false);
	printScreen();
}

std::unique_ptr<GameState::Base> Over::handleEvents()
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
			if ((event.key.code == game.keys.getKey("player shoot")) && (game.timestep.getTime() <= gameOverKeyPressDelay))
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

std::unique_ptr<Base> Over::update()
{
	return nullptr;
}

void Over::draw()
{
	game.window.draw(game.graphics);
	game.window.draw(game.cs);
}

void Over::printScreen()
{
	game.cs.clear();
	
	game.cs << Cs::StretchType::Both << Cs::Location(getColumnToCenterString(game.cs, gameOverString), game.cs.getMode().y / 2u - 1u) << gameOverString << Cs::StretchType::None;
	if (game.timestep.getTime() > gameOverKeyPressDelay)
		game.cs << Cs::Location(getColumnToCenterString(game.cs, continueString), 20) << continueString;
}

} // namespace GameState
