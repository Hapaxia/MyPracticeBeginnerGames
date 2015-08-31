#include "Graphics.hpp"
#include <Plinth/Sfml/Anchor.hpp>
#include <Plinth/Generic.hpp>

Graphics::Graphics(Resources& resources) :
m_ball(resources.getTexture("spritesheet")),
m_player(resources.getTexture("spritesheet")),
m_opponent(resources.getTexture("spritesheet")),
m_paddleReachFromCenter(0.f),
m_paddleWidth(20.f)
{
	m_player.setTextureRect({ sf::Vector2i{ 0, 0 }, sf::Vector2i{ 60, 100 } });
	m_opponent.setTextureRect({ sf::Vector2i{ 0, 100 }, sf::Vector2i{ 60, 100 } });
	m_ball.setTextureRect({ sf::Vector2i{ 0, 200 }, sf::Vector2i{ 24, 24 } });
	using namespace pl::Anchor::Local;
	m_ball.setOrigin(getCenter(m_ball));
	m_player.setOrigin(getCenterLeft(m_player));
	m_opponent.setOrigin(getCenterRight(m_opponent));
}

void Graphics::updatePlayer(const sf::View& view, const Paddle& player)
{
	updateSize(view);
	m_player.setPosition(sf::Vector2f{ 0.f, (m_paddleReachFromCenter * player.getPosition()) + (view.getSize().y / 2) });
}

void Graphics::updateOpponent(const sf::View& view, const Paddle& opponent)
{
	updateSize(view);
	m_opponent.setPosition(sf::Vector2f{ view.getSize().x, (m_paddleReachFromCenter * opponent.getPosition()) + (view.getSize().y / 2) });
}

void Graphics::updateBall(const Ball& ball, const float dt)
{
	m_ball.setPosition(ball.getPosition());
	m_ball.rotate(0.125f * ball.getSpin() * ball.getSpeed() * dt);
}

void Graphics::updateSize(const sf::View& view)
{
	m_paddleReachFromCenter = view.getSize().y * 0.4f;
}

bool Graphics::playerCollidesWith(const sf::FloatRect& bounds) const
{
	return m_player.getGlobalBounds().intersects(bounds);
}

bool Graphics::opponentCollidesWith(const sf::FloatRect& bounds) const
{
	return m_opponent.getGlobalBounds().intersects(bounds);
}

bool Graphics::isInLineWithPlayer(const float y) const
{
	return pl::inRange(y, pl::Range<float>{pl::Anchor::Global::getTopCenter(m_player).y, pl::Anchor::Global::getBottomCenter(m_player).y});
}

bool Graphics::isHigherThanPlayer(const float y) const
{
	return y < pl::Anchor::Global::getTopCenter(m_player).y;
}

bool Graphics::isLowerThanPlayer(const float y) const
{
	return y > pl::Anchor::Global::getBottomCenter(m_player).y;
}

bool Graphics::isToTheRightOfPlayer(float x) const
{
	return x > pl::Anchor::Global::getCenterRight(m_player).x;
}

bool Graphics::isToTheRightOfPlayerBack(float x) const
{
	return x > (pl::Anchor::Global::getCenterRight(m_player).x - m_paddleWidth);
}

bool Graphics::isInLineWithOpponent(const float y) const
{
	return pl::inRange(y, pl::Range<float>{pl::Anchor::Global::getTopCenter(m_opponent).y, pl::Anchor::Global::getBottomCenter(m_opponent).y});
}

bool Graphics::isHigherThanOpponent(const float y) const
{
	return y < pl::Anchor::Global::getTopCenter(m_opponent).y;
}

bool Graphics::isLowerThanOpponent(const float y) const
{
	return y > pl::Anchor::Global::getBottomCenter(m_opponent).y;
}

bool Graphics::isToTheLeftOfOpponent(float x) const
{
	return x < pl::Anchor::Global::getCenterLeft(m_opponent).x;
}

bool Graphics::isToTheLeftOfOpponentBack(float x) const
{
	return x < (pl::Anchor::Global::getCenterLeft(m_opponent).x + m_paddleWidth);
}

float Graphics::getPlayerFront() const
{
	return pl::Anchor::Global::getCenterRight(m_player).x;
}

float Graphics::getPlayerTop() const
{
	return pl::Anchor::Global::getTopCenter(m_player).y;
}

float Graphics::getPlayerBottom() const
{
	return pl::Anchor::Global::getBottomCenter(m_player).y;
}

float Graphics::getOpponentFront() const
{
	return pl::Anchor::Global::getCenterLeft(m_opponent).x;
}

float Graphics::getOpponentTop() const
{
	return pl::Anchor::Global::getTopCenter(m_opponent).y;
}

float Graphics::getOpponentBottom() const
{
	return pl::Anchor::Global::getBottomCenter(m_opponent).y;
}

float Graphics::getBallDistanceLeftOfOpponent() const
{
	return pl::Anchor::Global::getCenterLeft(m_opponent).x - m_ball.getPosition().x;
}

float Graphics::getPaddleReachFromCenter() const
{
	return m_paddleReachFromCenter;
}

void Graphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_opponent);
	target.draw(m_player);
	target.draw(m_ball);
}
