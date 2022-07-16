// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "./Map.hpp"
#include "./Terrain.hpp"

class MapWriter
{
public:
	friend class Map;

	explicit MapWriter(const Map& source)
			:source(new Map{ source })
	{
	}

	MapWriter(size_t id, Matrix<char> area, size_t finalTime)
			:source(new Map{ id, std::move(area), finalTime, START_TIME })
	{
	}

	[[nodiscard]] Map write() const;

	bool step();

private:
	static constexpr size_t START_TIME{ 0 };

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

	friend Point operator+(const Point& l, const Point& r);

	friend bool operator>(const Point& l, const Point& r);

	friend bool operator<(const Point& l, const Point& r);

	[[nodiscard]] char nextTerrain(const Point& location) const;

	[[nodiscard]] size_t countNeighbors(const char& value,
			const Point& location, size_t limit) const;

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
		size_t rows{ source->rows };
		size_t cols{ source->cols };
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
	return { source->rows - 1, source->cols - 1 };
}

#pragma clang diagnostic pop