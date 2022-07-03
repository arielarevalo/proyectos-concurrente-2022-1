// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <string>

#include "./file/FileWatcher.hpp"
#include "./solver/Filer.hpp"
#include "./solver/Solver.hpp"

class TetrisPlayer
{
public:
	/**
 * @brief Starts watching the target directory.
 * @details Watches a directory using the inotify library to continuously
 * monitor a specific folder and, on given file system events, check
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
				History highScore{ Solver::solve(gameState) };
				Logger::info("Successfully found best moves for game state.");

				Filer::write(highScore);
				Logger::info("Successfully wrote best moves to files.");
			}
			catch (const std::invalid_argument&)
			{
				std::throw_with_nested(
						std::invalid_argument(
								"Failed to validate input file values.")
				);
			}
			catch (const std::out_of_range&)
			{
				std::throw_with_nested(
						std::out_of_range(
								"Failed to validate input file dimensions.")
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
			catch (const std::exception& e)
			{
				Logger::error("Solver has crashed.", e);
			}
		}
	}
}