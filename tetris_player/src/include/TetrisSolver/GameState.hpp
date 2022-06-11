// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "./Matrix.hpp"
#include "./Tetrimino.hpp"

/**
 * The set of initial game state information.
 */
class GameState
{
public:
	/**
	 *
	 * @param id
	 * @param depth
	 * @param playArea
	 * @param nextTetriminos
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
	Matrix playArea;
	const std::vector<Tetrimino::Figure> nextTetriminos;
};
