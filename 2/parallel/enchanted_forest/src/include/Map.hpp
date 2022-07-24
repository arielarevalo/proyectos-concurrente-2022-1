// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <string>
#include <utility>

#include "./Matrix.hpp"

/**
 * @brief A single immutable work unit of a map file starting area and its
 * processing characteristics.
 */
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
	/**
	 * @brief Constructs a Map instance that succeeds an original one.
	 * @param map Map to succeed.
	 * @param area New area for successor.
	 * @param currentTime Updated time for successor.
	 */
	Map(const Map& map, Matrix<char> area, size_t currentTime)
			:Map(map.id, std::move(area), map.isTraced,
			map.finalTime, currentTime)
	{
	}

	/**
	 * @brief Constructs an original Map instance.
	 * @param id Id for new Map.
	 * @param area Area for new Map.
	 * @param isTraced Whether every intermediate step should be printed for
	 * this map.
	 * @param finalTime Final midnight for this map.
	 * @param currentTime Starting midnight for this map.
	 */
	Map(std::string id, Matrix<char> area, bool isTraced,
			size_t finalTime, size_t currentTime)
			:id(std::move(id)), area(std::move(area)),
			 isTraced(isTraced), finalTime(finalTime), currentTime(currentTime)
	{
	}
};
