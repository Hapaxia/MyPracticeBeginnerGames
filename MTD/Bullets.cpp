#include "Bullets.hpp"

#include <Dev.hpp>
#include <Plinth/StringFrom.hpp>

Bullets::Bullets()
	: m_retriggerDelay(0.4) // retrigger delay in seconds
	, m_speed(1000.0)
	, m_bullets()
	, m_retriggerTimer(0.0)
{
}

void Bullets::shoot(const pl::Vector2d position)
{
	if (m_retriggerTimer >= 0.0)
		return;

	m_bullets.emplace_back(Bullet(position));
	m_retriggerTimer = m_retriggerDelay;
}

void Bullets::update(const double dt)
{
	if (m_retriggerTimer > -1.0)
		m_retriggerTimer -= dt;

	for (std::list<Bullet>::iterator it = m_bullets.begin(), end = m_bullets.end(); it != end;)
	{
		it->update(m_speed * dt);
		if (it->getPosition().y < 0.0)
			it = m_bullets.erase(it);
		else
			++it;
	}
}
