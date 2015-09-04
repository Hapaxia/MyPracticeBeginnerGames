#include "Music.hpp"

//#include <Plinth/Generic.hpp>

Music::Music():
m_track(Track::Ready),
m_pMusic(&m_ready),
m_isDimmed(false)
{
	const std::string resourcePath = "resources/";
	// music used with permission by Hapax Perplexia (https://soundcloud.com/hapaxperplexia/puzza-soundtrack)
	if (!m_ready.openFromFile(resourcePath + "puzza-ready.ogg") ||
		!m_play.openFromFile(resourcePath + "puzza-play.ogg"))
		throw("Could not find music.");

	m_ready.setLoop(true);
	m_play.setLoop(true);
}

void Music::play(Track track)
{
	m_track = track;
	m_pMusic->stop();
	switch (m_track)
	{
	case Track::Play:
		m_pMusic = &m_play;
		break;
	case Track::Ready:
	default:
		m_pMusic = &m_ready;
	}
	m_pMusic->setVolume(0.f);
	m_pMusic->play();
	m_timer.restart();
}

void Music::update()
{
	const kairos::Duration fadeInLength(2.0); // in seconds
	float volume = 100.f;
	if (m_timer.getTime() < fadeInLength)
		volume = static_cast<float>((m_timer.getTime().asSeconds() / fadeInLength.asSeconds()) * 100.0);
	if (m_isDimmed)
		volume /= 2.f;
	m_pMusic->setVolume(volume);
}

void Music::dim()
{
	m_isDimmed = true;
}

void Music::undim()
{
	m_isDimmed = false;
}
