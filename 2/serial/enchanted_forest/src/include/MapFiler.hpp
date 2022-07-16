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

using Job = std::vector<Map>;

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
			outputPath(inputPath + OUTPUT)
	{
	}

	Job parseJob();

	void file(const Map& map);

private:
	static constexpr char DASH{ '-' };

	static constexpr char MAP[4]{ "map" };

	static constexpr char TXT[5]{ ".txt" };

	static constexpr char OUTPUT[8]{ "output/" };

	static constexpr char JOB_REGEX[16]{ "job[0-9]{3}.txt" };

	static constexpr char TASK_REGEX[25]{ "map[0-9]{3}.txt -*[0-9]+" };

	static constexpr size_t FILENAME_SIZE{ 10 };

	static constexpr size_t ID_START{ 3 };

	static constexpr size_t ID_SIZE{ 3 };

	static std::string parseInputPath(const std::string& jobPath);

	Map parseMap(const std::string& task);

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

Job MapFiler::parseJob()
{
	std::ifstream file{ jobPath };

	file.exceptions(
			std::ifstream::badbit | std::ifstream::failbit);

	Job job;
	while (file && !file.eof())
	{
		std::string task;
		std::getline(file, task);

		Map map{ parseMap(task) };
		job.push_back(map);
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

	size_t id{ std::stoul(filename.substr(ID_START, ID_SIZE)) };

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
			if (!file.eof())
			{
				Point current{ i, j };
				file.get(area[current]);
				if (std::string{ Terrain::legalChars }.find(area[current])
						== std::string::npos)
				{
					throw std::invalid_argument("Terrain at ("
							+ std::to_string(current.first) + ","
							+ std::to_string(current.second)
							+ ") has an invalid value: " + value);
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

void MapFiler::file(const Map& map)
{
	std::string filename{
			outputPath + MAP + std::to_string(map.id)
					+ DASH + std::to_string(map.currentTime) + TXT };

	std::ofstream file{ filename };

	file.exceptions(std::ofstream::badbit | std::ifstream::failbit);

	for (size_t i{ 0 }; i < map.rows; ++i)
	{
		for (size_t j{ 0 }; j < map.cols; j++)
		{
			Point current{ i, j };
			file << map.area[current];
		}
		file << std::endl;
	}
}
