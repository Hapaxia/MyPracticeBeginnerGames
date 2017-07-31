#ifndef MTD_GAMESTATE_OVER_HPP
#define MTD_GAMESTATE_OVER_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Over final : public Base
{
public:
	Over(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents() final override;
	virtual std::unique_ptr<Base> update() final override;
	virtual void draw() final override;
	virtual void printScreen() final override;
};

} // namespace GameState
#endif // MTD_GAMESTATE_OVER_HPP
