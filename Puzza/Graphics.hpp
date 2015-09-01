#ifndef PUZZA_GRAPHICS_HPP
#define PUZZA_GRAPHICS_HPP

#include "Resources.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

class Graphics : public sf::Drawable
{
public:

	Graphics(Resources& resources);
	void updatePlayer(const sf::View& view, const Paddle& player);
	void updateOpponent(const sf::View& view, const Paddle& opponent);
	void updateBall(const Ball& ball, float dt);
	void updateSize(const sf::View& view);

	bool playerCollidesWith(const sf::FloatRect& bounds) const;
	bool opponentCollidesWith(const sf::FloatRect& bounds) const;

	bool isInLineWithPlayer(float y) const;
	bool isHigherThanPlayer(float y) const;
	bool isLowerThanPlayer(float y) const;

	bool isToTheRightOfPlayer(float x) const;
	bool isToTheRightOfPlayerBack(float x) const;

	bool isInLineWithOpponent(float y) const;
	bool isHigherThanOpponent(float y) const;
	bool isLowerThanOpponent(float y) const;

	bool isToTheLeftOfOpponent(float x) const;
	bool isToTheLeftOfOpponentBack(float x) const;

	float getPlayerFront() const;
	float getPlayerTop() const;
	float getPlayerBottom() const;

	float getOpponentFront() const;
	float getOpponentTop() const;
	float getOpponentBottom() const;

	float getBallDistanceLeftOfOpponent() const;

	float getPaddleReachFromCenter() const;

	void startBallAnimHit();

private:
	const float m_paddleWidth;

	float m_paddleReachFromCenter;
	sf::Sprite m_ball, m_player, m_opponent;

	bool m_animBallHit;

	float m_animFrameBall;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PUZZA_GRAPHICS_HPP