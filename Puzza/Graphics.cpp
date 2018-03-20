#include "Graphics.hpp"
#include <Plinth/Sfml/Anchor.hpp>
#include <Plinth/Generic.hpp>
#include <Plinth/Range.hpp>

Graphics::Graphics(Resources& resources) :
m_ball(resources.getTexture("spritesheet")),
m_player(resources.getTexture("spritesheet")),
m_opponent(resources.getTexture("spritesheet")),
m_animBallHit(false),
m_animPlayerHit(false),
m_animOpponentHit(false),
m_animFrameBall(0.f),
m_animFramePlayer(0.f),
m_animFrameOpponent(0.f),
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

void Graphics::updatePlayer(const sf::View& view, const Paddle& player, const float dt)
{
	updateSize(view);
	m_player.setPosition(sf::Vector2f{ 0.f, (m_paddleReachFromCenter * player.getPosition()) + (view.getSize().y / 2) });

	if (!m_animPlayerHit)
		return;

	const float playerAnimSpeed = 24.f; // fps

	m_animFramePlayer += playerAnimSpeed * dt;

	if (m_animFramePlayer >= 16.f)
	{
		m_animFramePlayer = 0.f;
		m_animPlayerHit = false;
	}

	m_player.setTextureRect({ sf::Vector2i{ static_cast<int>(m_animFramePlayer) * 60, 0 }, sf::Vector2i{ 60, 100 } });
}

void Graphics::updateOpponent(const sf::View& view, const Paddle& opponent, const float dt)
{
	updateSize(view);
	m_opponent.setPosition(sf::Vector2f{ view.getSize().x, (m_paddleReachFromCenter * opponent.getPosition()) + (view.getSize().y / 2) });

	if (!m_animOpponentHit)
		return;

	const float opponentAnimSpeed = 24.f; // fps

	m_animFrameOpponent += opponentAnimSpeed * dt;

	if (m_animFrameOpponent >= 16.f)
	{
		m_animFrameOpponent = 0.f;
		m_animOpponentHit = false;
	}

	m_opponent.setTextureRect({ sf::Vector2i{ static_cast<int>(m_animFrameOpponent) * 60, 100 }, sf::Vector2i{ 60, 100 } });
}

void Graphics::updateBall(const Ball& ball, const float dt)
{
	m_ball.setPosition(ball.getPosition());
	m_ball.rotate(0.125f * ball.getSpin() * ball.getSpeed() * dt);

	const float ballAnimSpeed = 18.f; // fps

	m_animFrameBall += ballAnimSpeed * dt;

	if (m_animBallHit)
	{
		if (m_animFrameBall < 8.f)
			m_animFrameBall = 8.f;
		else if (m_animFrameBall >= 16.f)
			m_animBallHit = false;
	}

	if (!m_animBallHit && m_animFrameBall >= 8.f)
		m_animFrameBall = 0.f;

	m_ball.setTextureRect({ sf::Vector2i{ static_cast<int>(m_animFrameBall) * 24, 200 }, sf::Vector2i{ 24, 24 } });
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
	return pl::Range<float>{ pl::Anchor::Global::getTopCenter(m_player).y, pl::Anchor::Global::getBottomCenter(m_player).y }.contains(y);
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
	return pl::Range<float>{ pl::Anchor::Global::getTopCenter(m_opponent).y, pl::Anchor::Global::getBottomCenter(m_opponent).y }.contains(y);
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

void Graphics::startBallAnimHit()
{
	m_animBallHit = true;
}

void Graphics::startPlayerAnimHit()
{
	m_animPlayerHit = true;
}

void Graphics::startOpponentAnimHit()
{
	m_animOpponentHit = true;
}

void Graphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_opponent);
	target.draw(m_player);
	target.draw(m_ball);
}
