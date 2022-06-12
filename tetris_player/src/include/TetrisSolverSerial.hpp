// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "TetrisSolver/Filer.hpp"
#include "TetrisSolver/GameState.hpp"
#include "TetrisSolver/Solver.hpp"

class TetrisSolverSerial
{
public:
	static bool play(std::ifstream& file);
};

bool TetrisSolverSerial::play(std::ifstream& file)
{
	try
	{
		GameState initial{ Filer::read(file) };
		Logger::info("Successfully read initial game state from file.");

		Logger::setStart();

		Solver solver{ initial };
		std::vector<PlayState> history{ solver.getBestMoves() };
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
