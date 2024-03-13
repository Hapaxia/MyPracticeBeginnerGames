#ifndef MTD_LEVEL_HPP
#define MTD_LEVEL_HPP

#include <vector>
#include <Plinth/Vector2.hpp>
//#include <SFML/System/Vector2.hpp>

class Level
{
public:
	Level();

	const std::vector<unsigned char>& getData() const;
	pl::Size2<unsigned long int> getSize() const;
	unsigned long int getWidth() const;
	unsigned long int getHeight() const;

	void reset();
	void setTile(pl::Vector2u position, unsigned char tileValue);
	unsigned char getTile(pl::Vector2u position) const;
	void reduceTileHealth(pl::Vector2u position, unsigned int healthReduction);
	unsigned int getTileHealth(pl::Vector2u position) const;

	bool isCoordInRange(pl::Vector2d coord) const;
	pl::Vector2u getTilePositionAtCoord(pl::Vector2d coord) const;
	unsigned int getVerticalOffset() const;

private:
	std::vector<unsigned char> m_data;
	pl::Size2<unsigned long int> m_size;
};

inline const std::vector<unsigned char>& Level::getData() const
{
	return m_data;
}

inline pl::Size2<unsigned long int> Level::getSize() const
{
	return m_size;
}

inline unsigned long int Level::getWidth() const
{
	return m_size.width;
}

inline unsigned long int Level::getHeight() const
{
	return m_size.height;
}

inline void Level::setTile(const pl::Vector2u position, const unsigned char tileValue)
{
	m_data[static_cast<std::size_t>(position.y) * m_size.width + position.x] = tileValue;
}

inline unsigned char Level::getTile(const pl::Vector2u position) const
{
	return m_data[static_cast<std::size_t>(position.y) * m_size.width + position.x];
}

#endif // MTD_LEVEL_HPP
