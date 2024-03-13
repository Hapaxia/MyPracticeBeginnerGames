#include "GameState_Play.hpp"

#include "gameStates.hpp"
#include "functions.hpp"

namespace GameState
{

Play::Play(Game& passedGame)
	: Base(passedGame)
{
	game.timestep.unpause();
	game.cs.setShowBackground(false);
	printScreen();
}

std::unique_ptr<GameState::Base> Play::handleEvents()
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
				return std::make_unique<GameState::Pause>(game);
			else if (event.key.code == game.keys.getKey("toggle enemy direction"))
				game.enemies.toggleDirection();
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == game.keys.getKey("player shoot"))
				game.allowPlayerBulletFire = true;
			break;
		}
	}
	return nullptr;
}

std::unique_ptr<Base> Play::update()
{
	// input
	int controlDirection = 0;
	if (sf::Keyboard::isKeyPressed(game.keys.getKey("player left")))
		--controlDirection;
	if (sf::Keyboard::isKeyPressed(game.keys.getKey("player right")))
		++controlDirection;
	if (game.allowPlayerBulletFire && sf::Keyboard::isKeyPressed(game.keys.getKey("player shoot")))
		game.bullets.shoot({ game.player.getPosition(), game.window.getView().getSize().y });

	// update
	if (controlDirection != 0)
	{
		if (controlDirection > 0)
			game.player.move(Player::Direction::Right);
		else
			game.player.move(Player::Direction::Left);
	}
	game.bullets.update(game.timestep.getStep());
	game.enemies.update(game.timestep.getStep(), game.player.getPosition());

	enum class Progression
	{
		None,
		EnemiesWon,
		EnemiesDestroyed,
		PlayerDeath
	} progression = Progression::None;

	// prepare player's bounding box
	pl::Vector2d playerHalfSize{ game.player.getSize().x / 2.0, game.player.getSize().y };
	pl::RangeArea<double> playerBoundingBox;
	playerBoundingBox.setLeftTop(pl::Vector2d{ game.player.getPosition(), game.window.getView().getSize().y } - playerHalfSize);
	playerBoundingBox.setRightBottom({ game.player.getPosition() + playerHalfSize.x, game.window.getView().getSize().y });

	// remove enemies hit by bullets
	pl::RangeArea<double> enemyBoundingBox;
	pl::RangeArea<double> bulletBoundingBox;
	pl::Vector2d enemyHalfSize;
	pl::Vector2d bulletHalfSize;
	std::vector<unsigned int> enemiesToRemove;
	std::vector<unsigned int> bulletsToRemove;
	for (auto enemy = game.enemies.begin(), enemiesEnd = game.enemies.end(); enemy != enemiesEnd; ++enemy)
	{
		if (!enemy->isAlive())
			continue;
		enemyHalfSize = enemy->getSize() / 2.0;
		enemyBoundingBox.setLeftTop(enemy->getPosition() - enemyHalfSize);
		enemyBoundingBox.setRightBottom(enemy->getPosition() + enemyHalfSize);
		if (enemyBoundingBox.overlaps(playerBoundingBox))
		{
			progression = Progression::EnemiesWon;
			break;
		}
		unsigned int bulletNumber{ 0u };
		for (auto& bullet : game.bullets)
		{
			if (bullet.isAlive())
			{
				bulletHalfSize = bullet.getSize() / 2.0;
				bulletBoundingBox.setLeftTop(bullet.getPosition() - pl::Vector2d{ bulletHalfSize.x, bulletHalfSize.y * 2.0 });
				bulletBoundingBox.setRightBottom(bullet.getPosition() + pl::Vector2d{ bulletHalfSize.x, 0.0 });
				if (enemyBoundingBox.overlaps(bulletBoundingBox))
				{
					addElementToVectorIfUnique(enemiesToRemove, enemy - game.enemies.begin());
					addElementToVectorIfUnique(bulletsToRemove, bulletNumber);
				}
			}
			++bulletNumber;
		}
	}

	// update high score
	if (game.score > game.highScore)
		game.highScore = game.score;

	// kill player if in contact with enemy bullet
	bool isPlayerDead{ false };
	for (auto enemy{ game.enemies.begin() }, enemiesEnd{ game.enemies.end() }; enemy != enemiesEnd; ++enemy)
	{
		const pl::Vector2d bulletPosition{ enemy->getBulletPosition() };
		const pl::Vector2d bulletSize{ enemy->getBulletSize() };
		const pl::Vector2d bulletCornerOffsetFromCenter{ bulletSize / 2.0 };

		// prepare enemy bullet's bounding box
		pl::RangeArea<double> bulletBoundingBox;
		bulletBoundingBox.setLeftTop(bulletPosition - bulletCornerOffsetFromCenter);
		bulletBoundingBox.setRightBottom(bulletPosition + bulletCornerOffsetFromCenter);

		if (enemy->isBulletAlive() && playerBoundingBox.overlaps(bulletBoundingBox))
		{
			isPlayerDead = true;
			enemy->killBullet(); // remove bullet that came into contact with player
			break;
		}
	}

	// test enemy bullets collision with covers
	for (auto enemy{ game.enemies.begin() }, enemiesEnd{ game.enemies.end() }; enemy != enemiesEnd; ++enemy)
	{
		const pl::Vector2d bulletPosition{ enemy->getBulletPosition() };
		const pl::Vector2d bulletSize{ enemy->getBulletSize() };
		const pl::Vector2d bulletCornerOffsetFromCenter{ bulletSize / 2.0 };
		const pl::Vector2d bulletBottomCenter{ bulletPosition.x, bulletPosition.y + bulletCornerOffsetFromCenter.y };

		if (enemy->isBulletAlive())
		{
			if (game.level.isCoordInRange(bulletBottomCenter))
			{
				const pl::Vector2u tilePosition{ game.level.getTilePositionAtCoord(bulletBottomCenter) };
				if (game.level.getTileHealth(tilePosition) > 0)
				{
					for (unsigned int y{ 0u }; y <= tilePosition.y; ++y)
						game.level.setTile({ tilePosition.x, y }, 0u);
					game.level.reduceTileHealth({ tilePosition.x - 2, tilePosition.y }, 1u);
					game.level.reduceTileHealth({ tilePosition.x + 2, tilePosition.y }, 1u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y }, 2u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y }, 2u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y - 1 }, 2u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y - 1 }, 2u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y + 1 }, 1u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y + 1 }, 1u);
					game.level.reduceTileHealth({ tilePosition.x, tilePosition.y + 1 }, 2u);
					enemy->killBullet();
				}
			}
		}
	}

	// test player bullets collision with covers
	unsigned int bulletNumber{ 0u };
	for (auto& bullet : game.bullets)
	{
		if (bullet.isAlive())
		{
			const pl::Vector2d bulletPosition{ bullet.getPosition() };
			const pl::Vector2d bulletSize{ bullet.getSize() };
			const pl::Vector2d bulletCornerOffsetFromCenter{ bulletSize / 2.0 };
			const pl::Vector2d bulletTopCenter{ bulletPosition.x + bulletCornerOffsetFromCenter.x, bulletPosition.y };

			if (game.level.isCoordInRange(bulletTopCenter))
			{
				const pl::Vector2u tilePosition{ game.level.getTilePositionAtCoord(bulletTopCenter) };
				if (game.level.getTileHealth(tilePosition) > 0)
				{
					if (tilePosition.y < game.level.getHeight() - 1)
					{
						for (unsigned int y{ tilePosition.y }; y < game.level.getHeight(); ++y)
							game.level.setTile({ tilePosition.x, y }, 0u);
					}
					game.level.reduceTileHealth({ tilePosition.x - 2, tilePosition.y }, 1u);
					game.level.reduceTileHealth({ tilePosition.x + 2, tilePosition.y }, 1u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y }, 2u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y }, 2u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y + 1 }, 2u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y + 1 }, 2u);
					game.level.reduceTileHealth({ tilePosition.x - 1, tilePosition.y - 1 }, 1u);
					game.level.reduceTileHealth({ tilePosition.x + 1, tilePosition.y - 1 }, 1u);
					game.level.reduceTileHealth({ tilePosition.x, tilePosition.y - 1 }, 2u);
					addElementToVectorIfUnique(bulletsToRemove, bulletNumber);
				}
			}
		}
		++bulletNumber;
	}

	// remove unused entities
	for (auto& enemyToRemove : enemiesToRemove)
	{
		game.enemies.killEnemy(enemyToRemove);
		game.score += 10;
		game.score += static_cast<unsigned int>(game.enemies.getDropSpeed() * 100 + game.enemies.getSpeed() / 50);
	}
	for (auto& bulletToRemove : bulletsToRemove)
		game.bullets.killBullet(bulletToRemove);

	// decide on the progression
	if (game.enemies.getNumberOfEnemiesAlive() == 0)
		progression = Progression::EnemiesDestroyed;
	else if (game.enemies.getReachedBottom())
		progression = Progression::EnemiesWon;
	else if (isPlayerDead)
	{
		if (--game.lives > 0u)
			progression = Progression::PlayerDeath;
		else
			progression = Progression::EnemiesWon;
	}

	// graphics
	game.graphics.updateView(game.window.getView());
	game.graphics.updatePlayer(game.player);
	game.graphics.updateBullets(game.bullets);
	game.graphics.updateEnemies(game.enemies);
	game.graphics.updateLevel();

	// progress state
	switch (progression)
	{
	case Progression::EnemiesDestroyed:
		game.timestep.resetTime();
		return std::make_unique<GameState::Win>(game);
	case Progression::EnemiesWon:
		game.timestep.resetTime();
		return std::make_unique<GameState::Over>(game);
	case Progression::PlayerDeath:
		return std::make_unique<GameState::Death>(game);
	default:
		return nullptr;
	}
}

void Play::draw()
{
	game.window.draw(game.graphics);
	game.window.draw(game.cs);
}

void Play::printScreen()
{
	game.cs.clear();
	game.cs << Cs::CursorCommand::Home << "SCORE " << pl::padStringLeft(pl::stringFrom(game.score), 4, '0');
	game.cs << Cs::Location(game.cs.getMode().x - 9, 0) << pl::padStringLeft(pl::stringFrom(game.highScore), 4, '0') << " HIGH";

	const std::string livesString{ "LIVES: " + pl::stringFrom(game.lives) };
	game.cs << Cs::Location(getColumnToCenterString(game.cs, livesString) + 1u, 0u) << livesString;
}

} // namespace GameState
