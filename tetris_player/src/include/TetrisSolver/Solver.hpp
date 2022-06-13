// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <vector>
#include <utility>
#include "./GameState.hpp"
#include "./PlayState.hpp"
#include "./Logger.hpp"

/**
 * @details Solver Class
 */
class Solver
{
public:
	/**
	 * @brief Determines the top scoring moves for the current play state.
	 * @details Navigates a tree of all possible states to find the series of moves
	 * that achieve the highest score possible.
	 */
	std::vector<PlayState>& getBestMoves();

	explicit Solver(const GameState& gameState)
			:gameState(gameState)
	{
	}

private:
	const int INITIAL_DEPTH{ -1 };

	bool findBestMoves(const PlayState& current, int currentDepth);

	bool findChildren(const PlayState& parent, int parentDepth);

	/**
	 * @brief Get the score of the current game state and make the comparison with a previous game state
	 * @details Get the score of the current game state and make the comparison with a previous game state
	 * if the current game state is higher it replaces the previous game state in the high score history
	 * @param current: PlayState
	 * @return bool
	 */
	bool compareToBest(const PlayState& current);

	const GameState& gameState;
	std::vector<PlayState> history{};
};

/**
 * @brief Determines the top scoring moves for the current play state.
 * @details Navigates a tree of all possible states to find the series of moves
 * that achieve the highest score possible.
 */
std::vector<PlayState>& Solver::getBestMoves()
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
}

/**
 * @brief Recursive method that performs a deep search based on depth in of the best moves
 * @details Recursive method that performs a deep search based on depth in of the best moves
 * the best scores are stored in an array of the history of the best plays
 * @param current: PlayState
 * @param currentDepth: int
 * @return isHighScore: bool
 */
bool Solver::findBestMoves(const PlayState& current, int currentDepth)
{
	int finalDepth{ static_cast<int>(gameState.depth) };
	bool isHighScore{ false };

	if (currentDepth < finalDepth)
	{
		isHighScore = findChildren(current, currentDepth);
		if (currentDepth > -1 && isHighScore)
		{
			history.push_back(current);
		}
	}
	else if (currentDepth == finalDepth)
	{
		isHighScore = compareToBest(current);
	}
	return isHighScore;
}

/**
 * @brief Recursive method that performs a deep search in search of the best score
 * @details Recursive method that performs a deep search in search of the best score
 * makes an attempt to place the piece with its rotations based on a game board and its children
 * @param parent: PlayState
 * @param parentDepth: int
 * @return isHighScore: bool
 */
bool Solver::findChildren(const PlayState& parent, int parentDepth)
{
	bool isHighScore{ false };

	int currentDepth{ parentDepth + 1 };

	Tetrimino::Figure nextTetrimino{ gameState.nextTetriminos[currentDepth] };
	size_t rotations{ Tetrimino::getTetriminoRotations(nextTetrimino) };

	for (size_t r{ 0 }; r < rotations; ++r)
	{
		for (size_t c{ 0 }; c < parent.getPlayArea().cols; ++c)
		{
			bool isChildHighScore{ false };

			if (PlayState temp{ parent }; temp.place(nextTetrimino, r, c))
			{
				isChildHighScore = findBestMoves(temp, currentDepth);
			}

			if (isChildHighScore)
			{
				isHighScore = true;
			}
		}
	}
	return isHighScore;
}

/**
 * @brief Get the score of the current game state and make the comparison with a previous game state
 * @details Get the score of the current game state and make the comparison with a previous game state
 * if the current game state is higher it replaces the previous game state in the high score history
 * @param current: PlayState
 * @return isHighScore: bool
 */
bool Solver::compareToBest(const PlayState& current)
{
	bool isHighScore{ false };

	if (history.empty())
	{
		history.push_back(current);
		isHighScore = true;
	}
	else
	{
		unsigned int currentScore{ current.score() };
		unsigned int topScore{ history.front().score() };

		if (currentScore > topScore)
		{
			Logger::info("New High Score:");
			std::cout << "\t" << currentScore << std::endl;
			current.getPlayArea().print();
			Logger::info("Old High Score:");
			std::cout << "\t" << topScore << std::endl;
			history.front().getPlayArea().print();

			history.clear();
			history.push_back(current);
			isHighScore = true;
		}
	}
	return isHighScore;
}