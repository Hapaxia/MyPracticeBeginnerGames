#ifndef MTD_GAMESTATE_OVER_HPP
#define MTD_GAMESTATE_OVER_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Over : public Base
{
public:
	Over(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents();
	virtual std::unique_ptr<Base> update();
	virtual void draw();
	virtual void printScreen();
};

} // namespace GameState
#endif // MTD_GAMESTATE_OVER_HPP
