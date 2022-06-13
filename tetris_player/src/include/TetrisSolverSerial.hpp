// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "Filer.hpp"
#include "GameState.hpp"
#include "Solver.hpp"

/**
 * @brief Solves a given game state by finding the best moves and outputting
 * them to files.
 */
class TetrisSolverSerial
{
public:
	/**
	 * @brief Finds the game state for a file and solves for the best moves.
 	 * @details Reads the file, processes file content, determines the
 	 * optimal move, and generates the output files.
	 * @param file Input game state file.
	 */
	static bool solve(std::ifstream& file);
};

bool TetrisSolverSerial::solve(std::ifstream& file)
{
	try
	{
		GameState initial{ Filer::read(file) };
		Logger::info("Successfully read initial game state from file.");

		Logger::setStart();

		Solver solver{ initial };
		std::vector<PlayState> history{ solver.solveBestMoves() };
		Logger::info("Successfully found best moves for game state.");

		Filer::write(history);
		Logger::info("Successfully wrote output files for moves.");

		return true;
	}
	catch (const std::invalid_argument&)
	{
		std::throw_with_nested(
				std::invalid_argument("Failed to validate input file values.")
		);
	}
	catch (const std::out_of_range&)
	{
		std::throw_with_nested(
				std::out_of_range("Failed to validate input file dimensions.")
		);
	}
	catch (const std::domain_error&)
	{
		std::throw_with_nested(
				std::domain_error("Failed to find best moves.")
		);
	}
	catch (const std::ios::failure&)
	{
		std::throw_with_nested(
				std::ios::failure("Failed to open/close file: " +
						std::string(std::strerror(errno)))
		);
	}
}
