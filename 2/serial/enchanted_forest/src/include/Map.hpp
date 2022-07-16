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
	const size_t finalTime;
	const size_t currentTime{ 0 };

private:
	Map(const Map& map, Matrix<char> area, size_t currentTime)
	: Map(map.id, std::move(area), map.finalTime, currentTime) {}

	Map(size_t id, Matrix<char> area, size_t finalTime, size_t currentTime)
			:id(id), rows(area.rows), cols(area.cols), finalTime(finalTime),
			 currentTime(currentTime), area(std::move(area))
	{
	}
};
