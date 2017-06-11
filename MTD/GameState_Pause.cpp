#include "GameState_Pause.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

namespace
{

const std::string pausedString{ "PAUSED!" };
const std::string continueString{ "Press P to continue" };

} // namespace

namespace GameState
{

Pause::Pause(Game& passedGame)
	: Base(passedGame)
{
	game.timestep.pause();
	game.cs.setShowBackground(false);
	printScreen();
}

std::unique_ptr<GameState::Base> Pause::handleEvents()
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
			else if (event.key.code == game.keys.getKey("pause"))
				return std::make_unique<GameState::Play>(game);
			break;
		}
	}
	return nullptr;
}

std::unique_ptr<Base> Pause::update()
{
	return nullptr;
}

void Pause::draw()
{
	game.window.draw(game.graphics);
	game.window.draw(game.cs);
}

void Pause::printScreen()
{
	game.cs.clear();

	game.cs << Cs::StretchType::Both << Cs::Location(getColumnToCenterString(game.cs, pausedString), game.cs.getMode().y / 2u - 1u) << pausedString << Cs::StretchType::None;
	game.cs << Cs::Location(getColumnToCenterString(game.cs, continueString), 20) << continueString;
}

} // namespace GameState
