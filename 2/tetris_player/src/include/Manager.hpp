// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include  <unistd.h>
#include <vector>
#include <memory>

#include "./GameState.hpp"
#include "./PlayState.hpp"
#include "./Solver.hpp"

using History = std::vector<PlayState>;

/**
 * @brief Directs threads to collectively solve a game state.
 */
class Manager
{
public:
	/**
	 * @brief Constructs a Manaer instance for a given game state.
	 * @param gameState Game state to solve for.
	 */
	explicit Manager(const GameState& gameState)
			:gameState(gameState)
	{
	}

	/**
	 * @brief Begins solving for the best moves for game state.
	 */
	History solveBestMoves();

private:
	/**
	 * @brief Makes the seperate initial game states for each subproblem.
	 * @return A vector of these initial game states.
	 */
	std::vector<GameState> makeInitials();

	/**
	 * @brief Makes a solver for each subproblem game state.
	 * @param initials The vector initial game states.
	 * @return A vector of one solver for each game state.
	 */
	std::vector<Solver>
	makeSolvers(const std::vector<GameState>& initials) const;

	/**
	 * @brief Reduces a vector of results to its top scoring member.
	 * @details This method was not implemented as an OpenMP reduction due to
	 * the difficulty of working with some of the custom data types created
	 * for this solver, as well as the necessity of empty case handling.
	 * @param results The vector of results to reduce.
	 * @return The top scoring element in results.
	 */
	History getBest(std::vector<History>& results) const;

	const GameState& gameState;

	std::vector<PlayState> initialPSs{};
};

History Manager::solveBestMoves()
{
	std::vector<GameState> initials{ makeInitials() };

	std::vector<Solver> solvers{ makeSolvers(initials) };

	std::vector<History> results{ solvers.size() };

#pragma omp parallel for
	for (size_t i = 0 ; i < initials.size(); ++i)
	{
		Solver& solver{ solvers[i] };
		History history{ solver.solveBestMoves() };
		History& result{ results[i] };
		result.swap(history);
	}

	return getBest(results);
}

std::vector<GameState> Manager::makeInitials()
{
	Tetrimino::Figure firstTetrimino{ gameState.nextTetriminos[0] };
	size_t rotations{ Tetrimino::getTetriminoRotations(firstTetrimino) };
	size_t columns{ gameState.playArea.cols };

	std::vector<GameState> initials{};

	for (size_t i{ 0 }; i < rotations; ++i)
	{
		for (size_t j{ 0 }; j < columns; ++j)
		{
			PlayState playState{ gameState };
			if (playState.place(firstTetrimino, i, j))
			{
				initialPSs.emplace_back(playState);
				initials.emplace_back(playState.getId(),
						gameState.depth,
						playState.getPlayArea(),
						gameState.nextTetriminos);
			}
		}
	}
	return initials;
}

std::vector<Solver>
Manager::makeSolvers(const std::vector<GameState>& initials) const
{
	std::vector<Solver> solvers{};

	for (const GameState& gs : initials)
	{
		solvers.emplace_back(gs);
	}
	return solvers;
}

History Manager::getBest(std::vector<History>& results) const
{
	History history{};
	for (size_t i{ 0 }; i < results.size(); ++i)
	{
		if (history.empty() || results[i].front() > history.front())
		{
			history.swap(results[i]);
			history.push_back(initialPSs[i]);
		}
	}
	return history;
}

