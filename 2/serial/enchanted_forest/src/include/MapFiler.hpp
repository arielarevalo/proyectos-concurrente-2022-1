// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "./Map.hpp"
#include "./MapWriter.hpp"
#include "./Terrain.hpp"

using Job = std::vector<std::string>;

/**
 * @brief Reads input file to game state and writes to output file with
 * history of best play state.
 */
class MapFiler
{
public:
	explicit MapFiler(const std::string& jobPath)
			:
			jobPath(jobPath), inputPath(parseInputPath(jobPath)),
			outputPath(parseOutputPath(jobPath))
	{
	}

	Job parseJob();

	Map parseMap(const std::string& task);

	void file(const Map& map) const;

private:
	static constexpr char DASH{ '-' };

	static constexpr char SLASH{ '/' };

	static constexpr char MAP[4]{ "map" };

	static constexpr char TXT[5]{ ".txt" };

	static constexpr char OUTPUT[8]{ "output" };

	static constexpr char JOB_REGEX[16]{ "job[0-9]{3}.txt" };

	static constexpr char TASK_REGEX[25]{ "map[0-9]{3}.txt -*[0-9]+" };

	static constexpr size_t FILENAME_SIZE{ 10 };

	static constexpr size_t ID_START{ 3 };

	static constexpr size_t ID_SIZE{ 3 };

	static std::string parseInputPath(const std::string& jobPath);

	static std::string parseOutputPath(const std::string& jobPath);

	const std::string jobPath;

	const std::string inputPath;

	const std::string outputPath;
};

std::string MapFiler::parseInputPath(const std::string& jobPath)
{
	std::string filename{
			jobPath.substr(jobPath.size() - FILENAME_SIZE, FILENAME_SIZE) };

	if (!std::regex_match(filename, std::regex(JOB_REGEX)))
	{
		throw std::invalid_argument("Invalid job filename: " + filename);
	}

	return jobPath.substr(0, jobPath.size() - FILENAME_SIZE);
}

std::string MapFiler::parseOutputPath(const std::string& jobPath)
{
	std::string inputPath{ parseInputPath(jobPath) };

	std::string directory{
			jobPath.substr(jobPath.size() - FILENAME_SIZE,
					FILENAME_SIZE - std::string{TXT}.size()) };

	return inputPath + directory + SLASH;
}

Job MapFiler::parseJob()
{
	std::ifstream file{ jobPath };

	file.exceptions(
			std::ifstream::badbit | std::ifstream::failbit);

	Job job;
	while (file && !(file >> std::ws).eof())
	{
		std::string task;
		std::getline(file, task);
		job.push_back(task);
	}

	return job;
}

Map MapFiler::parseMap(const std::string& task)
{
	if (!std::regex_match(task, std::regex(TASK_REGEX)))
	{
		throw std::invalid_argument("Invalid map task: " + task);
	}

	std::string filename{ task.substr(0, FILENAME_SIZE) };

	std::ifstream file{ inputPath + filename };

	file.exceptions(
			std::ifstream::badbit | std::ifstream::failbit);

	std::string id{ filename.substr(ID_START, ID_SIZE) };

	size_t rows;
	size_t cols;

	file >> rows;
	file >> cols;

	if (rows <= 0 || cols <= 0)
	{
		throw std::out_of_range{ "Input dimensions too small." };
	}

	Matrix<char> area{ rows, cols };

	for (size_t i{ 0 }; i < rows; ++i)
	{
		for (size_t j{ 0 }; j < cols; j++)
		{
			if (!(file >> std::ws).eof())
			{
				Point point{ i, j };
				char& current{ area[point] };

				file.get(current);
				if (std::string{ Terrain::legalChars }.find(current)
						== std::string::npos)
				{
					throw std::invalid_argument("Terrain at ("
							+ std::to_string(point.first) + ","
							+ std::to_string(point.second)
							+ ") has an invalid current: " + current);
				}
			}
			else
			{
				throw std::out_of_range("Invalid file contents: " + filename);
			}
		}
	}

	std::string timeStr{ task.substr(FILENAME_SIZE + 1,
			task.size() - FILENAME_SIZE + 1) };

	int timeInt{ std::stoi(timeStr) };

	size_t finalTime{ static_cast<size_t>(abs(timeInt)) };

	bool isTraced{ timeInt > 0 };

	MapWriter writer{ id, area, isTraced, finalTime };

	return writer.write();
}

void MapFiler::file(const Map& map) const
{
	if (map.isTraced || map.currentTime == map.finalTime)
	{
		std::filesystem::create_directory(outputPath);

		std::string filename{
				outputPath + MAP + map.id
						+ DASH + std::to_string(map.currentTime) + TXT };

		std::ofstream file{ filename };

		file.exceptions(std::ofstream::badbit | std::ifstream::failbit);

		for (size_t i{ 0 }; i < map.area.rows; ++i)
		{
			for (size_t j{ 0 }; j < map.area.cols; j++)
			{
				Point current{ i, j };
				file << map.area[current];
			}
			file << std::endl;
		}
	}
}
