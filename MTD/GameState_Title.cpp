#include "GameState_Title.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

namespace
{

const std::vector<std::string> infoTitle
{
	{ "M" },
	{ " T" },
	{ "  D " }
};
const std::vector<std::string> infoKeys
{
	{ " left \x10" },
	{ "right \x11" },
	{ " fire SPACE" },
	{ " quit ESC" },
	{ "pause P" }
};
const std::string startString{ "Press FIRE to start" };

constexpr double startKeyPressDelay{ 1.0 }; // seconds

} // namespace

namespace GameState
{

Title::Title(Game& passedGame)
	: Base(passedGame)
{
	game.timestep.resetTime();
	game.allowPlayerBulletFire = false;
	game.cs.setShowBackground(true);
	printScreen();
}

std::unique_ptr<GameState::Base> Title::handleEvents()
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
				game.quit();
			else if (event.key.code == game.keys.getKey("player shoot"))
			{
				if (game.timestep.getTime() <= startKeyPressDelay)
					break;
				game.reset();
				return std::make_unique<GameState::Play>(game);
			}
			break;
		}
	}
	return nullptr;
}

std::unique_ptr<Base> Title::update()
{
	return nullptr;
}

void Title::draw()
{
	game.window.draw(game.cs);
}

void Title::printScreen()
{
	game.cs.clear();
	unsigned int row{ 2u };
	game.cs << Cs::StretchType::Both;
	for (auto& line : infoTitle)
	{
		game.cs << Cs::Location(getColumnToCenterString(game.cs, infoTitle.back()), row++) << line;
		++row;
	}
	game.cs << Cs::StretchType::None;
	row = 11u;
	for (auto& line : infoKeys)
		game.cs << Cs::Location(10, row++) << line;
	if ((blink(game.timestep, 1.0, 1.5)) && (game.timestep.getTime() > startKeyPressDelay))
		game.cs << Cs::Location(getColumnToCenterString(game.cs, startString), 20) << startString;
}

} // namespace GameState
