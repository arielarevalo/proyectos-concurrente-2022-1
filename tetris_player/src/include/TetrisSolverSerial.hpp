// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "TetrisSolver/Filer.hpp"
#include "TetrisSolver/GameState.hpp"
#include "TetrisSolver/Solver.hpp"

/**
 * @details TetrisSolverSerial Class
 */
class TetrisSolverSerial {
public:
	/**
	 * @brief Method responsible for encapsulating the main logic of the system.
 	 * @details The following actions are executed: read the file, Process file content,
	 * algorithm to determine the optimal move, generate the output file(s)
	 * @param file: ifstream, a valid tetris_state.txt file
	 * 
	 */
    static void play(std::ifstream& file);
};

/**
 * @brief Method responsible for encapsulating the main logic of the system.
 * @details The following actions are executed: read the file, Process file content,
 * algorithm to determine the optimal move, generate the output file(s)
 * @param file: ifstream, a valid tetris_state.txt file
 * 
 */
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
