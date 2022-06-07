// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

/*
 * credits -> https://solarianprogrammer.com/2019/01/13/cpp-17-filesystem-write-file-watcher-monitor/
 * https://github.com/sol-prog/cpp17-filewatcher
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include "./TetrisSolverSerial.hpp"
#include "./Logger.hpp"

class FileWatcher
{
public:
	FileWatcher();

	void start();

private:
	void listFiles();

	const std::string putPath{ "./put" };
	const std::string targetFileName{ putPath + "/tetris_state.txt" };

	std::vector<std::filesystem::path> paths{};
	bool running{ true };
};

FileWatcher::FileWatcher()
{
	std::filesystem::remove_all(putPath);
	std::filesystem::create_directory(putPath);

	listFiles();
}

void FileWatcher::listFiles()
{
	paths.clear();

	for (auto& file : std::filesystem::recursive_directory_iterator(
			putPath))
	{
		paths.push_back(file.path());
	}
}

void FileWatcher::start()
{
	while (running)
	{
		listFiles();

		for (const std::filesystem::path& path : paths)
		{
			if (std::filesystem::exists(path)
					&& std::filesystem::is_regular_file(
							std::filesystem::path(path)) && path == targetFileName)
			{
				Logger::setStart();
				Logger::info("Found tetris game state file.");
				std::ifstream file{ path };
				TetrisSolverSerial::play(file);
				std::filesystem::remove(path);
			}
		}
	}
}
