#ifndef PUZZA_MUSIC_HPP
#define PUZZA_MUSIC_HPP

#include <SFML/Audio/Music.hpp>
#include <Kairos/Stopwatch.hpp>

class Music
{
public:
	enum class Track
	{
		Ready,
		Play
	};
	Music();
	void play(Track track);
	void update();
	void dim();
	void undim();

private:
	bool m_isDimmed;
	Track m_track;
	sf::Music* m_pMusic;
	sf::Music m_ready, m_play;
	kairos::Stopwatch m_timer; // for fade in
};

#endif // PUZZA_MUSIC_HPP
