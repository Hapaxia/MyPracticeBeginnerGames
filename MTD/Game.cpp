#include "Game.hpp"

#include "gameStates.hpp"

namespace
{

const std::string windowTitle{ "MTD" };
constexpr double timeStepSize{ 0.01 };

std::unique_ptr<GameState::Base> pGameState{ nullptr };

} // namespace

Game::Game()
	: timestep()
	, window(sf::VideoMode(800, 600), windowTitle, sf::Style::Default)
	, isGameRunning{ true }
	, graphics()
	, player(window, timeStepSize, graphics)
	, bullets()
	, enemies(window)
	, keys()
	, resources()
	, cs()
	, allowPlayerBulletFire(false)
	, score(0u)
{
	initKeys();
	if (!initResources())
		throw "Failed to initialise resources.";
	if (!initConsoleScreen())
		throw "Failed to initialise Console Screen.";
	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	timestep.setStep(timeStepSize);
	cs.clear();

	pGameState = std::make_unique<GameState::Title>(*this);
}

void Game::handleEvents()
{
	std::unique_ptr<GameState::Base> pState{ pGameState->handleEvents() };
	if (pState != nullptr)
		pGameState = std::move(pState);
}

void Game::update()
{
	timestep.addFrame();
	while (timestep.isUpdateRequired())
	{
		std::unique_ptr<GameState::Base> pState{ pGameState->update() };
		if (pState != nullptr)
			pGameState = std::move(pState);
	}
	pGameState->printScreen();
}

void Game::draw()
{
	window.clear();
	pGameState->draw();
	window.display();
}

void Game::initKeys()
{
	keys.addKey(sf::Keyboard::Left, "player left");
	keys.addKey(sf::Keyboard::Right, "player right");
	keys.addKey(sf::Keyboard::Space, "player shoot");
	keys.addKey(sf::Keyboard::P, "pause");
	keys.addKey(sf::Keyboard::Escape, "quit");

	// "power" controls (for debugging/testing)
	keys.addKey(sf::Keyboard::Return, "toggle enemy direction");
	//keys.addKey(sf::Keyboard::Tab, "progress state");
}

bool Game::initResources()
{
	try
	{
		resources.addTexture("font sheet", "resources/fontsheet.png");
	}
	catch (pl::Exception&)
	{
		return false;
	}
	return true;
}

bool Game::initConsoleScreen()
{
	const sf::Vector2u textureTileSize{ 8u, 8u };
	const sf::Vector2u consoleScreenScale{ 3u, 3u };
	cs.setTexture(resources.getTexture("font sheet"));
	cs.setTextureTileSize(textureTileSize);
	cs.setNumberOfTextureTilesPerRow(16);
	cs.setMode({ 33, 25 });
	cs.setSize({ static_cast<float>(cs.getMode().x * textureTileSize.x * consoleScreenScale.x), static_cast<float>(cs.getMode().y * textureTileSize.y * consoleScreenScale.y) });
	cs.setPosition((window.getSize().x - cs.getSize().x) / 2, 0);
	cs.setShowCursor(false);
	cs.setShowBackground(false);
	cs.crash();

	return true;
}

void Game::reset()
{
	timestep.resetTime();
	player.reset();
	enemies.reset();
	bullets.reset();
	graphics.clearEnemies();
	graphics.clearBullets();
	score = 0;
}
