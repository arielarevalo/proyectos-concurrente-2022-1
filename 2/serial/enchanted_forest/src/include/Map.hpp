// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>

#include "./Matrix.hpp"
#include "./Terrain.hpp"

class Map
{
public:
	Map(const Map& source)
			:id(source.id), rows(source.rows), cols(source.cols), time
			(source.time + 1), area(rows, cols)
	{
	}

	Map(std::string id, size_t rows, size_t cols, Matrix<char> area)
			:id(id), rows(rows), cols(cols), area(std::move(area))
	{
	}

	char& operator[](Point p);

	[[nodiscard]] char getNextTerrain(const Point& location) const;

	const std::string id;
	const size_t rows;
	const size_t cols;
	const size_t time{ 0 };

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

	[[nodiscard]] size_t countNeighbors(const char& value,
			const Point& location, size_t limit) const;

	const Point max{ rows - 1, cols - 1 };

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

char& Map::operator[](Point p)
{
	return area[p];
}

char Map::getNextTerrain(const Point& location) const
{
	switch (const char& value{ area[location] })
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

size_t Map::countNeighbors(const char& value,
		const Point& location, size_t limit) const
{
	size_t valueCount{ 0 };

	for (const Point& p : scope)
	{
		Point next{ location + p };
		if (next > min && next < max && area[next] == value)
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
