// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>

#include "./Matrix.hpp"
#include "./Terrain.hpp"

using Point = std::pair<int, int>;

class Map
{
public:
	Map(size_t rows, size_t cols, Matrix<char> area)
			:rows(rows), cols(cols), area(std::move(area))
	{
	}

	void step();

	size_t getTime();

	const size_t rows;
	const size_t cols;

private:
	friend Point operator+(const Point& l, const Point& r);

	friend bool operator>(const Point& l, const Point& r);

	friend bool operator<(const Point& l, const Point& r);

	static constexpr Point min{ 0, 0 };

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

	void updateTerrain(const Point& location);

	size_t
	countNeighbors(const char& value, const Point& location, size_t limit);

	const Point max{ rows - 1, cols - 1 };

	size_t time{ 0 };
	Matrix<char> area;
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

void Map::step()
{
	for (size_t i{ 0 }; i < rows; i++)
	{
		for (size_t j{ 0 }; j < cols; j++)
		{
			Point current{ i, j };
			updateTerrain(current);
		}
	}
	++time;
}

size_t Map::getTime()
{
	return time;
}

void Map::updateTerrain(const Point& location)
{
	switch (char& value{ area[location.first][location.second] })
	{
	case Terrain::tree:
		if (countNeighbors(Terrain::lake, location, 4) > 3)
		{
			value = Terrain::lake;
		}
		else if (countNeighbors(Terrain::tree, location, 5) > 4)
		{
			value = Terrain::meadow;
		}
		return;
	case Terrain::lake:
		if (countNeighbors(Terrain::lake, location, 3) < 3)
		{
			value = Terrain::meadow;
		}
		return;
	case Terrain::meadow:
		if (countNeighbors(Terrain::tree, location, 3) > 2)
		{
			value = Terrain::tree;
		}
		return;
	default:
		throw std::invalid_argument("Terrain at ("
				+ std::to_string(location.first) + ","
				+ std::to_string(location.second)
				+ ") has an invalid value: " + value);
	}
}

size_t
Map::countNeighbors(const char& value, const Point& location, size_t limit)
{
	size_t valueCount{ 0 };

	for (const Point& p : scope)
	{
		Point next{ location + p };
		if (next > min && next < max
		&& area[next.first][next.second] == value)
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
