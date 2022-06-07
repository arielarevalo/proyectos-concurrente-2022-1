// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "TetrisSolver/Filer.hpp"
#include "TetrisSolver/GameState.hpp"
#include "TetrisSolver/Solver.hpp"

class TetrisSolverSerial {
public:
    static void play(std::ifstream& file);
};

void TetrisSolverSerial::play(std::ifstream& file)
{
	try
	{
		GameState initial{ Filer::read(file) };
		Logger::info("Successfully read initial game state from file.");

		Solver solver{ initial };
		std::vector<PlayState> history{solver.getBestMoves()};
		Logger::info("Successfully found best moves for game state.");

		Filer::write(history);
		Logger::info("Successfully wrote output files for moves.");

	} catch (const std::invalid_argument& ia) {
		Logger::error("Was not able to read input file. " +
				std::string(ia.what()));
	} catch (const std::domain_error& de) {
		Logger::error("Was not able to find best moves. " +
		std::string(de.what()));
	} catch (const std::ios::failure& io) {
		Logger::error("Was not able to open/close file. " +
				std::string(io.what()));
	}
}
