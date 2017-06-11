#ifndef MTD_GAMESTATE_HPP
#define MTD_GAMESTATE_HPP

#include "Game.hpp"

#include <memory>

namespace GameState
{

class Base
{
public:
	Base(Game& passedGame) : game(passedGame) { }

	virtual std::unique_ptr<Base> handleEvents() = 0;
	virtual std::unique_ptr<Base> update() = 0;
	virtual void draw() = 0;
	virtual void printScreen() = 0;

protected:
	Game& game;
};

} // namespace GameState
#endif // MTD_GAMESTATE_HPP
