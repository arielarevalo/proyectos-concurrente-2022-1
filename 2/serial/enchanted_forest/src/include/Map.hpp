// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>

#include "./Matrix.hpp"
#include "./MapWriter.hpp"

class Map
{
public:
	friend class MapWriter;

	const size_t id;
	const size_t rows;
	const size_t cols;
	const Matrix<char> area;
	const bool isTraced;
	const size_t finalTime;
	const size_t currentTime{ 0 };

private:
	Map(const Map& map, Matrix<char> area, size_t currentTime)
			:Map(map.id, std::move(area), map.isTraced,
			map.finalTime, currentTime)
	{
	}

	Map(size_t id, Matrix<char> area, bool isTraced,
			size_t finalTime, size_t currentTime)
			:id(id), rows(area.rows), cols(area.cols), area(std::move(area)),
			 isTraced(isTraced), finalTime(finalTime), currentTime(currentTime)
	{
	}
};
