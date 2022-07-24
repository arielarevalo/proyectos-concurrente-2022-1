// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "./Map.hpp"
#include "./Terrain.hpp"

/**
 * @brief Handles writing new Map files, due to their immutability.
 * @details Externalizing updates to Map files was a decision made
 * to simplifying paralallelization further down the line.
 */
class MapWriter
{
public:
	friend class Map;

	/**
     * @brief Constructs a MapWriter with a copy of a map as its source.
     * @param source Map to copy.
     */
	explicit MapWriter(const Map& source)
			:source(new Map{ source })
	{
	}

	/**
	* @brief Constructs a MapWriter with an original map as its source.
	* @param id Id for original map to use as source.
	* @param area Area for original map to use as source.
	* @param isTraced Traceability for original map to use as source.
	* @param finalTime Total midnights for original map to use as source.
	*/
	MapWriter(std::string id, Matrix<char> area, bool isTraced, size_t finalTime)
			:source(new Map{ std::move(id), std::move(area), isTraced,
							 finalTime, START_TIME })
	{
	}

	/**
     * @brief Returns a copy of source map.
     * @return Copy of source map.
     */
	[[nodiscard]] Map write() const;

	/**
     * @brief Updates source map to its next iteration.
     * @return Whether map has not reached its final iteration.
     */
	bool step();

private:
	static constexpr size_t START_TIME{ 0 };

	static constexpr Point min{ -1, -1 };

	static constexpr Point scope[8]{
			{ -1, 0 },
			{ -1, 1 },
			{ 0, 1 },
			{ 1, 1 },
			{ 1, 0 },
			{ 1, -1 },
			{ 0, -1 },
			{ -1, -1 }
	};

	friend Point operator+(const Point& l, const Point& r);

	friend bool operator>(const Point& l, const Point& r);

	friend bool operator<(const Point& l, const Point& r);

	/**
     * @brief Calculates the next terrain for a given point in source.
     * @param location Point for which to find next terrain.
     * @return Next terrain character.
     */
	[[nodiscard]] char nextTerrain(const Point& location) const;

	/**
     * @brief Counts the number of neighbors of a given terrain around a point
     * up to a given limit.
     * @param value Terrain type to count.
     * @param location Point for which to count neighbors.
     * @param limit Highest value to count.
     * @return Number of neighbors counted.
     */
	[[nodiscard]] size_t countNeighbors(const char& value,
			const Point& location, size_t limit) const;

	/**
     * @brief Calculates maximum position for current source dimensions.
     * @return Maximum position.
     */
	[[nodiscard]] Point max() const;

	std::unique_ptr<Map> source;
};

Point operator+(const Point& l, const Point& r)
{
	return { l.first + r.first, l.second + r.second };
}

bool operator>(const Point& l, const Point& r)
{
	return l.first > r.first && l.second > r.second;
}

bool operator<(const Point& l, const Point& r)
{
	return l.first < r.first && l.second < r.second;
}

Map MapWriter::write() const
{
	return *source;
}

bool MapWriter::step()
{
	bool cont{ source->currentTime < source->finalTime };

	if (cont)
	{
		size_t rows{ source->area.rows };
		size_t cols{ source->area.cols };
		Matrix<char> destArea{ rows, cols };

		for (size_t i{ 0 }; i < rows; i++)
		{
			for (size_t j{ 0 }; j < cols; j++)
			{
				Point current{ i, j };
				char next{ nextTerrain(current) };
				destArea[current] = next;
			}
		}

		source.reset(new Map{ *source, destArea, source->currentTime + 1 });
	}

	return cont;
}

char MapWriter::nextTerrain(const Point& location) const
{
	switch (const char& value{ source->area[location] })
	{
	case Terrain::tree:
		if (countNeighbors(Terrain::lake, location, 4) > 3)
		{
			return Terrain::lake;
		}
		else if (countNeighbors(Terrain::tree, location, 5) > 4)
		{
			return Terrain::meadow;
		}
		return value;
	case Terrain::lake:
		if (countNeighbors(Terrain::lake, location, 3) < 3)
		{
			return Terrain::meadow;
		}
		return value;
	case Terrain::meadow:
		if (countNeighbors(Terrain::tree, location, 3) > 2)
		{
			return Terrain::tree;
		}
		return value;
	default:
		throw std::invalid_argument("Terrain at ("
				+ std::to_string(location.first) + ","
				+ std::to_string(location.second)
				+ ") has an invalid value: " + value);
	}
}

size_t MapWriter::countNeighbors(const char& value,
		const Point& location, size_t limit) const
{
	size_t valueCount{ 0 };

	for (const Point& p : scope)
	{
		Point next{ location + p };
		if (next > min && next < max() && source->area[next] == value)
		{
			++valueCount;
			if (valueCount >= limit)
			{
				break;
			}
		}
	}
	return valueCount;
}

Point MapWriter::max() const
{
	return { source->area.rows, source->area.cols };
}