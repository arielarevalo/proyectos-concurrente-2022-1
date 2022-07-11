// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <deque>
#include <string>

#include "./file/FileWatcher.hpp"
#include "./solver/Filer.hpp"
#include "./solver/Solver.hpp"

/**
 * @brief Executes a Tetris Player instance.
 */
class TetrisPlayer
{
public:
	/**
	 * @brief Starts watching the target directory.
	 * @details Watches a directory using the inotify library to continuously
	 * monitor a specific folder and, on given file system events, finalize
	 * for target file names and process the files.
	 */
	static void start();
};

void TetrisPlayer::start()
{
	Filer::initialize();

	FileWatcher fw{ Filer::INITIAL_PATH, Filer::TARGET, Filer::END_TARGET };

	Logger::info("Starting file watch loop.");
	while (true)
	{
		Logger::setStart();
		std::string path{ fw.watch() };

		if (path == std::string{ Filer::INITIAL_PATH }
				+ std::string{ Filer::END_TARGET })
		{
			Logger::info("Successfully found stop file. Finalizing.");
			return;
		}

		if (!path.empty())
		{
			Logger::info("Successfully found tetris game state file.");

			std::ifstream file{ path };

			file.exceptions(
					std::ifstream::badbit | std::ifstream::failbit);

			try
			{
				GameState gameState{ Filer::read(file) };
				Logger::info("Successfully read initial game state from file.");

				Logger::setStart();
				Logger::info("Finding best moves for game state.");
				std::deque<PlayState> history{ Solver::solve(gameState) };
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
}