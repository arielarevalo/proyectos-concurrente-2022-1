// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "GameState.h"
#include "PlayState.h"

class Filer
{
public:
	/**
	 * @brief Reads a specification file into an in_state.
	 * @details Builds an in_state structure at a given location from the contents of a file at a given location
	 * @param input Pointer to a file to read from
	 * @return
	 */
	static GameState read(std::ifstream& file);

	/**
	 * @brief Writes all play_states into specification output files.
	 * @details Builds files at a preset location based on the series of play_state memory locations passed to it
	 * @param history Array of pointers to direct descendants of a given play_state
	 * @param size Size of the array of pointers
	 * @return 1 if successful, 0 if unsuccessful
	 */
	static void write(const std::vector<PlayState>& history);
private:
	static const size_t BASE_FILENAME_SIZE;
	static const std::string VALID_TET_CHARS;

	static bool isValidTet(char tet);
};

const size_t Filer::BASE_FILENAME_SIZE{ 25 };
const std::string Filer::VALID_TET_CHARS{ "IZSLJOT" };

GameState Filer::read(std::ifstream& file)
{
	u_int64_t id{ 0 };
	file >> id;

	size_t depth{ 0 };
	file >> depth;

	size_t rows{ 0 };
	file >> rows;

	size_t cols{ 0 };
	file >> cols;

	if (rows < 4 || cols < 4)
	{
		throw std::invalid_argument{ "dimensions" };
	}

	Matrix playArea{ rows, cols };

	for (size_t i{ 0 }; i < rows; ++i)
	{
		file >> playArea[i];
	}

	size_t nextTetriminosSize{ 0 };
	file >> nextTetriminosSize;

	/* DEPTH MUST BE SMALLER THAN SIZE OF NEXT TETRIMINOS */
	if (depth >= nextTetriminosSize)
	{
		throw std::invalid_argument("depth");
	}

	std::string nextTetriminos;

	for (size_t i{ 0 }; i < nextTetriminosSize; ++i)
	{
		file >> nextTetriminos[i];
		if (!isValidTet(nextTetriminos[i]))
		{
			throw std::invalid_argument("tetriminos");
		}
	}

	return { id, depth, playArea, nextTetriminos };
}

void Filer::write(const std::vector<PlayState>& history)
{
	for (size_t i{ 0 }; i < history.size(); ++i)
	{
		std::string filename{ "./output/tetris_play_"
									  + std::to_string(i)
									  + ".txt" };

		const PlayState& current = history[i];

		std::ofstream file;

		file.open(filename);
		file << current.getId() << std::endl;
		file << current.getLastTetrimino() << std::endl;
		file << current.getLastRotation() << std::endl;
		current.getPlayArea().print(file);
		file.close();
	}
}

bool Filer::isValidTet(char tet)
{
	return std::any_of(VALID_TET_CHARS.begin(), VALID_TET_CHARS.end(),
			[&tet](char c){ return c == tet; });
}