// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <string>
#include <utility>

#include "./Matrix.hpp"

class Map
{
public:
	friend class MapWriter;

	const std::string id;
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

	Map(std::string id, Matrix<char> area, bool isTraced,
			size_t finalTime, size_t currentTime)
			:id(std::move(id)), area(std::move(area)),
			 isTraced(isTraced), finalTime(finalTime), currentTime(currentTime)
	{
	}
};
