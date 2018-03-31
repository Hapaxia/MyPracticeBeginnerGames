#include "Level.hpp"

#include <algorithm>
#include <cmath>

namespace
{

constexpr pl::Size2<unsigned long int> defaultSize{ 200u, 100u };
constexpr pl::Size2<unsigned int> tileSize{ 8u, 8u };
constexpr unsigned int verticalOffset{ 480u };
constexpr unsigned int tileHealthOffset{ 8u };
constexpr unsigned int tileStartOffset{ 1u };
constexpr unsigned int maximumTileHealth{ 4u };

} // namespace

Level::Level()
	: m_data(defaultSize.getArea())
	, m_size(defaultSize)
{
	reset();
}

void Level::reset()
{
	std::fill(m_data.begin(), m_data.end(), 0u);

	// build cover block roof
	constexpr unsigned int blockDataWidth{ 9u };
	constexpr unsigned int blockDataHeight{ 8u };
	std::vector<unsigned char> blockData
	{
		0, 2, 1, 1, 1, 1, 1, 3, 0,
		2, 1, 1, 1, 1, 1, 1, 1, 3,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 5, 0, 0, 0, 4, 1, 1,
		1, 5, 0, 0, 0, 0, 0, 4, 1,
	};

	constexpr unsigned int numberOfBlocks{ 4u };
	constexpr unsigned int initialOffset{ 16u };
	constexpr unsigned int separationWidth{ 11u };

	for (unsigned int y{ 0u }; y < blockDataHeight; ++y)
	{
		for (unsigned int x{ 0u }; x < blockDataWidth; ++x)
		{
			const unsigned char blockValue{ blockData[y * blockDataWidth + x] };
			for (unsigned int block{ 0u }; block < numberOfBlocks; ++block)
			{
				unsigned char& cell{ m_data[y * m_size.width + initialOffset + x + block * (separationWidth + blockDataWidth)] };
				cell = blockValue;
			}
		}
	}
}

void Level::reduceTileHealth(const pl::Vector2u position, const unsigned int healthReduction)
{
	if (getTile(position) == 0u)
		return;

	const unsigned char tileType{ (getTile(position) - tileStartOffset) % tileHealthOffset };
	const unsigned int initialTileHealth{ getTileHealth(position) };
	const unsigned int targetTileHealth{ (initialTileHealth > healthReduction) ? (initialTileHealth - healthReduction) : 0u };
	if (targetTileHealth == 0u)
	{
		setTile(position, 0u);
		return;
	}
	const unsigned int targetTileIndex{ tileStartOffset + tileType + (tileHealthOffset * (maximumTileHealth - targetTileHealth)) };
	setTile(position, targetTileIndex);
}

unsigned int Level::getTileHealth(pl::Vector2u position) const
{
	const unsigned char tileValue{ getTile(position) };
	if (tileValue == 0u)
		return 0u;
	return maximumTileHealth - ((tileValue - tileStartOffset) / tileHealthOffset);
}

bool Level::isCoordInRange(const pl::Vector2d coord) const
{
	const pl::Vector2d position{ coord.x / tileSize.width, (coord.y - verticalOffset) / tileSize.height };
	return !(position.x < 0 || position.y < 0 || position.x >= getWidth() || position.y >= getHeight());
}

pl::Vector2u Level::getTilePositionAtCoord(const pl::Vector2d coord) const
{
	const pl::Vector2d position{ coord.x / tileSize.width, (coord.y - verticalOffset) / tileSize.height };
	if (position.x < 0 || position.y < 0 || position.x >= getWidth() || position.y >= getHeight())
		return{ 0u, 0u };
	return{ static_cast<unsigned int>(std::floor(position.x) + 0.5), static_cast<unsigned int>(std::floor(position.y) + 0.5) };
}

unsigned int Level::getVerticalOffset() const
{
	return verticalOffset;
}
