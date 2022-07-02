// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "Matrix.hpp"
#include "./Tetrimino.hpp"

/**
 * @brief The set of initial game state information.
 */
class GameState
{
public:
	/**
	 * Constructs a game state from its required fields.
	 * @param id Identifier of the game state.
	 * @param depth Depth to solve this game state for.
	 * @param playArea Initial play area of the game state.
	 * @param nextTetriminos List of next tetriminos to place.
	 */
	GameState(uint64_t id,
			size_t depth,
			Matrix playArea,
			std::vector<Tetrimino::Figure> nextTetriminos)
			:
			id(id), depth(depth), playArea(std::move(playArea)),
			nextTetriminos(std::move(nextTetriminos))
	{
	}

	const uint64_t id;
	const size_t depth;
	const Matrix playArea;
	const std::vector<Tetrimino::Figure> nextTetriminos;
};
