#ifndef MTD_GAMESTATE_PAUSE_HPP
#define MTD_GAMESTATE_PAUSE_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Pause final : public Base
{
public:
	Pause(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents() final override;
	virtual std::unique_ptr<Base> update() final override;
	virtual void draw() final override;
	virtual void printScreen() final override;
};

} // namespace GameState
#endif // MTD_GAMESTATE_PAUSE_HPP
