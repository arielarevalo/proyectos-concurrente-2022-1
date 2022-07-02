/*
// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <queue>
#include <utility>
#include <vector>
#include "../common/History.hpp"
#include "../logger/Logger.hpp"

*/
/**
 * @brief Solves a game state for its best moves.
 *//*

class Solver
{
public:
	*/
/**
	 * @brief Determines the top scoring moves for the initial game state.
	 * @details Navigates a procedural tree of all possible states to find the
	 * series of moves that achieve the highest score possible.
	 *//*

	std::vector<PlayState>& solveBestMoves();

	*/
/**
	 * Constructs a Solver from a given game state.
	 * @param gameState Game state of the Solver.
	 *//*

	explicit Solver(const GameState& gameState)
			:gameState(gameState)
	{
	}

private:
	const int INITIAL_DEPTH{ -1 };


};

std::vector<PlayState>& Solver::solveBestMoves()
{
	PlayState initial{ gameState };

	findBestMoves(initial, INITIAL_DEPTH);

	if (!history.empty())
	{
		return history;
	}
	else
	{
		throw std::domain_error("No valid moves down to requested depth.");
	}
}*/
