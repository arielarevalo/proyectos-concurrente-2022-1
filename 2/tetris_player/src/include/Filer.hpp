// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include "GameState.hpp"
#include "PlayState.hpp"

/**
 * @brief Reads input file to game state and writes to output file with
 * history of best play state.
 */
class Filer
{
public:
	static constexpr char OUTPUT_PATH[7]{ "./put/" };

	/**
	 * Sets working directory to its initial state.
	 */
	static void initialize();

	/**
	 * @brief Reads input file into a new game state.
	 * @param file File to read from.
	 * @return New game state.
	 */
	static GameState read(std::ifstream& file);

	/**
	 * @brief Writes history of best play states into output files.
	 * @param history List of ancestors of best play state.
	 */
	static void write(std::vector<PlayState>& history);
};

void Filer::initialize()
{
	std::filesystem::remove_all(OUTPUT_PATH);
	std::filesystem::create_directory(OUTPUT_PATH);
}

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
		throw std::out_of_range{ "Input dimensions too small." };
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
		throw std::out_of_range("Input depth too high for tetriminos.");
	}

	std::vector<Tetrimino::Figure> nextTetriminos;

	for (size_t i{ 0 }; i < nextTetriminosSize; ++i)
	{
		char nextChar;
		file >> nextChar;

		Tetrimino::Figure nextTetrimino{ Tetrimino::charToEnum(nextChar) };
		nextTetriminos.push_back(nextTetrimino);
	}

	return { id, depth, playArea, nextTetriminos };
}

void Filer::write(std::vector<PlayState>& history)
{
	initialize();

	const size_t initialSize{ history.size() };
	for (size_t i{ 0 }; i < initialSize; ++i)
	{
		std::string filename{ "../bin/put/tetris_play_"
									  + std::to_string(i)
									  + ".txt" };

		PlayState current{ history.back() };
		history.pop_back();

		std::ofstream file{ filename };
		file.exceptions(std::ofstream::badbit | std::ifstream::failbit);

		file << current.getId() << std::endl;
		file << current.getLastTetrimino() << std::endl;
		file << current.getLastRotation() << std::endl;
		current.getPlayArea().print(file);
	}
}
