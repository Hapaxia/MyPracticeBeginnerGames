#ifndef MTD_GAMESTATE_PLAY_HPP
#define MTD_GAMESTATE_PLAY_HPP

#include "GameState.hpp"
#include "Game.hpp"

namespace GameState
{

class Play final : public Base
{
public:
	Play(Game& game);

	virtual std::unique_ptr<GameState::Base> handleEvents() final override;
	virtual std::unique_ptr<Base> update() final override;
	virtual void draw() final override;
	virtual void printScreen() final override;
};

} // namespace GameState
#endif // MTD_GAMESTATE_PLAY_HPP
