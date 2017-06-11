#ifndef MTD_GAMESTATE_PAUSE_HPP
#define MTD_GAMESTATE_PAUSE_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Pause : public Base
{
public:
	Pause(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents();
	virtual std::unique_ptr<Base> update();
	virtual void draw();
	virtual void printScreen();
};

} // namespace GameState
#endif // MTD_GAMESTATE_PAUSE_HPP
