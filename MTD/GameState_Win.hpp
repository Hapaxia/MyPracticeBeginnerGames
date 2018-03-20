#ifndef MTD_GAMESTATE_WIN_HPP
#define MTD_GAMESTATE_WIN_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Win final : public Base
{
public:
	Win(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents() final override;
	virtual std::unique_ptr<Base> update() final override;
	virtual void draw() final override;
	virtual void printScreen() final override;
};

} // namespace GameState
#endif // MTD_GAMESTATE_WIN_HPP
