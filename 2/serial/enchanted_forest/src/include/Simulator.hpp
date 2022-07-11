// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "Filer.hpp"
#include "GameState.hpp"
#include "Manager.hpp"
#include "Solver.hpp"
#include "Logger.hpp"

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
	static void solve(long numThreads, std::string path);
};

void Simulator::solve(long numThreads, std::string path)
{
	Logger::setStart();

	if (!path.empty())
	{
		std::ifstream file{ path };

		file.exceptions(
				std::ifstream::badbit | std::ifstream::failbit);

		try
		{
			GameState initial{ Filer::read(file) };
			Logger::info("Successfully read initial game state from file.");

			// loop: mientras para este mapa en el job falten noches
			// // map.step()
			// // filer.write(map)


			Logger::info("Finding best moves.");
			Manager manager{ initial };

			Logger::setStart();
			std::vector<PlayState> history{ manager.solveBestMoves() };
			Logger::info("Successfully found best moves for game state.");

			Filer::write(history);
			Logger::info("Successfully wrote best moves to files.");
		}
		catch (const std::invalid_argument& ia)
		{
			Logger::error("Failed to validate input file values.", ia);
		}
		catch (const std::out_of_range& oor)
		{
			Logger::error("Failed to validate input file dimensions.", oor);
		}
		catch (const std::domain_error& de)
		{
			Logger::error("Failed to find best moves.", de);
		}
		catch (const std::ios::failure& iof)
		{
			Logger::error("Failed to open/close file: "
					+ std::string(std::strerror(errno)), iof);
		}
		catch (const std::exception& e)
		{
			Logger::error("Solver has crashed.", e);
		}
	}
}