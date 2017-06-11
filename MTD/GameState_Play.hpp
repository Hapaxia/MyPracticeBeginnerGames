#ifndef MTD_GAMESTATE_PLAY_HPP
#define MTD_GAMESTATE_PLAY_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Play : public Base
{
public:
	Play(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents();
	virtual std::unique_ptr<Base> update();
	virtual void draw();
	virtual void printScreen();
};

} // namespace GameState
#endif // MTD_GAMESTATE_PLAY_HPP
