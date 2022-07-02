// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <string>

#include "./file/FileWatcher.hpp"
#include "./file/Filer.hpp"
#include "Solver.hpp"

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

	Logger::info("Starting file watching loop.");
	while (true)
	{
		std::string path{ FileWatcher::watch() };

		if (path == std::string{ Filer::INITIAL_PATH }
				+ std::string{ Filer::END_TARGET })
		{
			Logger::info("Finalizing.");
			exit(0);
		}

		if (!path.empty())
		{
			std::ifstream file{ path };

			file.exceptions(
					std::ifstream::badbit | std::ifstream::failbit);

			std::unique_ptr<GameState> gameState;
			try
			{
				gameState = std::make_unique<GameState>(Filer::read(file));
			}
			catch (const std::exception& e)
			{
				Logger::error("Unable to process file.", e);
			}

			try
			{
				Solver::processGameState(*gameState);
			}
			catch (const std::exception& e)
			{
				Logger::error("Solver has crashed.", e);
			}
		}
	}
}