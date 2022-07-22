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
			outputPath(parseOutputPath(jobPath))
	{
	}

	Job parseJob();

	void file(const Map& map) const;

private:
	static constexpr char DASH{ '-' };

	static constexpr char SLASH{ '/' };

	static constexpr char BREAK{ '\n' };

	static constexpr char MAP[4]{ "map" };

	static constexpr char TXT[5]{ ".txt" };

	static constexpr char OUTPUT[8]{ "output" };

	static constexpr char JOB_REGEX[16]{ "job[0-9]{3}.txt" };

	static constexpr char TASK_REGEX[25]{ "map[0-9]{3}.txt -*[0-9]+" };

	static constexpr int BCAST_ROOT{ 0 };

	static constexpr size_t FILENAME_SIZE{ 10 };

	static constexpr size_t ID_START{ 3 };

	static constexpr size_t ID_SIZE{ 3 };

	static std::string parseInputPath(const std::string& jobPath);

	static std::string parseOutputPath(const std::string& jobPath);

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

std::string MapFiler::parseOutputPath(const std::string& jobPath)
{
	std::string inputPath{ parseInputPath(jobPath) };

	std::string directory{
			jobPath.substr(jobPath.size() - FILENAME_SIZE,
					FILENAME_SIZE - std::string{ TXT }.size()) };

	return inputPath + directory + SLASH;
}

Job MapFiler::parseJob()
{
	int rank{ -1 };
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int charCount{ 0 };

	if (rank == BCAST_ROOT)
	{
		std::filesystem::remove_all(outputPath);
		std::filesystem::create_directory(outputPath);

		std::fstream fs{ jobPath };
		std::string line{};

		while (getline(fs, line))
		{
			charCount += line.length() + 1; // \n
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&charCount, 1, MPI_INT, BCAST_ROOT, MPI_COMM_WORLD);

	MPI_File file;
	MPI_File_open(MPI_COMM_WORLD, jobPath.data(), MPI_MODE_RDONLY,
			MPI_INFO_NULL, &file);

	std::string content(charCount, '\0');
	MPI_File_read(file, content.data(), charCount, MPI_CHAR, MPI_STATUS_IGNORE);

	MPI_File_close(&file);

	std::string temp{};
	std::stringstream ss{ content };
	std::vector<std::string> allTasks{};
	while (std::getline(ss, temp, BREAK))
	{
		allTasks.push_back(temp);
	}

	int processCount{ -1 };
	MPI_Comm_size(MPI_COMM_WORLD, &processCount);

	size_t remainder{ 0 };
	size_t taskCount{ allTasks.size() };
	remainder = taskCount % static_cast<size_t>(processCount);

	size_t mapsPerProcess{ taskCount / processCount };
	size_t rankSizeT{ static_cast<size_t>(rank) };
	size_t taskEnd{ (rankSizeT + 1) * mapsPerProcess };
	std::vector<std::string> myTasks(std::next(allTasks.begin(), rankSizeT),
			std::next(allTasks.begin(), taskEnd));

	size_t remainderStart{ taskCount - remainder };
	if (rankSizeT < remainder)
	{
		myTasks.push_back(allTasks[remainderStart + rank]);
	}

	Job job;
	for (std::string task : myTasks)
	{
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
	std::string filePath{ inputPath + filename };

	MPI_File file;
	MPI_File_open(MPI_COMM_WORLD, filePath.data(), MPI_MODE_RDONLY,
			MPI_INFO_NULL, &file);

	std::string id{ filename.substr(ID_START, ID_SIZE) };

	std::vector<size_t> dims(2);

	for (size_t& dim : dims)
	{
		std::string temp{};
		char next{ '0' };
		while (!isspace(next))
		{
			MPI_File_read(file, &next, 1, MPI_CHAR,
					MPI_STATUS_IGNORE);
			temp.push_back(next);
		}

		temp.pop_back();
		dim = std::stoul(temp);
	}

	size_t rows{ dims[0] };
	size_t cols{ dims[1] };

	if (rows <= 0 || cols <= 0)
	{
		throw std::out_of_range{ "Input dimensions too small." };
	}

	Matrix<char> area{ rows, cols };

	char next{ BREAK };
	for (size_t i{ 0 }; i < rows; ++i)
	{
		for (size_t j{ 0 }; j < cols; j++)
		{
			MPI_File_read(file, &next, 1, MPI_CHAR,
					MPI_STATUS_IGNORE);
			if (next != EOF)
			{
				Point point{ i, j };
				if (std::string{ Terrain::legalChars }.find(next)
						== std::string::npos)
				{
					throw std::invalid_argument("Terrain at ("
							+ std::to_string(point.first) + ","
							+ std::to_string(point.second)
							+ ") has an invalid current: " + next);
				}
				area[point] = next;
			}
			else
			{
				throw std::out_of_range("Invalid file contents: " + filename);
			}
		}
		MPI_File_seek(file, 1, MPI_SEEK_CUR); // \n
	}

	MPI_File_close(&file);

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
		std::string filename{
				outputPath + MAP + map.id
						+ DASH + std::to_string(map.currentTime) + TXT };

		MPI_File file;
		MPI_File_open(MPI_COMM_WORLD, filename.data(), MPI_MODE_CREATE | MPI_MODE_WRONLY,
				MPI_INFO_NULL, &file);

		std::string output{};
		for (size_t i{ 0 }; i < map.area.rows; ++i)
		{
			for (size_t j{ 0 }; j < map.area.cols; j++)
			{
				Point current{ i, j };
				output.push_back(map.area[current]);
			}
			output.push_back(BREAK);
		}

		MPI_File_write(file, output.data(), static_cast<int>(output.size()),
				MPI_CHAR, MPI_STATUS_IGNORE);

		MPI_File_close(&file);
	}
}
