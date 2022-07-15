// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "./Map.hpp"

/**
 * @brief Solves a given game state by finding the best moves and outputting
 * them to files.
 */
class Simulator
{
public:
	/**
	 * @brief Finds the game state for a file and solves for the best moves.
 	 * @details Reads the file, processes file content, determines the
 	 * optimal move, and generates the output files.
	 * @param file Input game state file.
	 */
	[[nodiscard]] Map step(const Map& source) const;
};

Map Simulator::step(const Map& source) const
{
	Map dest{ source };
	for (size_t i{ 0 }; i < source.rows; i++)
	{
		for (size_t j{ 0 }; j < source.cols; j++)
		{
			Point current{ i, j };
			char next{ source.getNextTerrain(current) };
			dest[current] = next;
		}
	}
	return dest;
}

#pragma clang diagnostic pop