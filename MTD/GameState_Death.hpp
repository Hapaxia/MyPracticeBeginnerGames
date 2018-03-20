#ifndef MTD_GAMESTATE_DEATH_HPP
#define MTD_GAMESTATE_DEATH_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace sf
{

class Clock;

} // namespace sf

namespace GameState
{

class Death final : public Base
{
public:
	Death(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents() final override;
	virtual std::unique_ptr<Base> update() final override;
	virtual void draw() final override;
	virtual void printScreen() final override;

private:
	sf::Clock m_clock;
};

} // namespace GameState
#endif // MTD_GAMESTATE_DEATH_HPP
