// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstdint>
#include <utility>
#include "./GameState.hpp"
#include "./Matrix.hpp"
#include "./Tetrimino.hpp"

/**
 * A single state of a game of Tetris.
 */
class PlayState
{
public:
	/**
	 * @brief Creates a new play_state structure.
	 * @details Allocates memory for a new play_state struct, builds it, and returns its location
	 * @return Pointer to location of newly created play_state struct
	 */
	explicit PlayState(const GameState& gameState)
			:id(gameState.id),
			 playArea(gameState.playArea)
	{
	}

	/**
	 * @brief Attempts to add a new tetrimino to the passed play_state.
	 * @details Attempts to add a new tetrimino to the play_area of the passed play_state at the given column
	 * @param play_state Pointer to play_state to update
	 * @param tetrimino Tetrimino to add to the play_state
	 * @param rotation Rotation of tetrimino to add
	 * @param column Column of the play_area at which to attempt to add the tetrimino
	 * @return Whether place was successful
	 */
	bool place(Tetrimino::Figure figure, size_t rotation, size_t column);

	unsigned int score() const;

	uint64_t getId() const;

	char getLastTetrimino() const;

	int getLastRotation() const;

	const Matrix& getPlayArea() const;

private:
	static const char INITIAL_TETRIMINO = static_cast<char>(32);  // Whitespace
	static const int INITIAL_ROTATION = -1;  // Invalid

	const uint64_t id;
	char lastTetrimino{ INITIAL_TETRIMINO };
	int lastRotation{ INITIAL_ROTATION };
	Matrix playArea;
};

bool PlayState::place(Tetrimino::Figure figure, size_t rotation, size_t column)
{
	const Tetrimino& tetrimino{ Tetrimino::getTetrimino(rotation, figure) };

	if (tetrimino.width > playArea.cols - column)
	{
		return false;
	}

	/* Confirm space is open for tet at the top */
	for (size_t j{ 0 }; j < tetrimino.width; ++j)
	{
		for (size_t i{ 0 }; i <= tetrimino.bounds[j]; ++i)
		{
			if ('0' != playArea[i][j + column])
			{
				return false;
			}
		}
	}

	/* With starting position at each column based on bounds, check at each
     * column if next downward cell is non-zero, if so, stop. At this point
     * "d" is the displacement of the tetrimino from the initial position. */
	size_t d{ 0 };
	bool cont{ true };
	for (;; ++d)
	{
		for (size_t j{ 0 }; j < tetrimino.width; ++j)
		{
			if (tetrimino.bounds[j] + d + 1 >= playArea.rows ||
					'0' != playArea[tetrimino.bounds[j] + d + 1][j + column])
			{
				cont = false;
			}
		}

		if (!cont)
		{  // Skip last ++d
			break;
		}
	}

	/* Paint the play_area with the nominal values of the tetrimino shape
	 * plus some displacement "d". */
	for (size_t i{ 0 }; i < tetrimino.height; ++i)
	{
		for (size_t j{ 0 }; j < tetrimino.width; ++j)
		{
			char currentValue{ tetrimino.value[i][j] };
			if ('0' != currentValue)
			{
				playArea[i + d][j + column] = currentValue;
			}
		}
	}

	lastRotation = rotation;
	lastTetrimino = tetrimino.character;

	return true;
}

unsigned int PlayState::score() const
{
	unsigned int score{ 0 };

	for (size_t i{ 0 }; i < playArea.rows; ++i)
	{
		unsigned int sum{ 0 };
		for (size_t j{ 0 }; j < playArea.cols; ++j)
		{
			if (playArea[i][j] != '0')
			{
				++sum;
			}
		}
		if (sum > 0)
		{
			score += sum * i * i;
		}
	}

	return score;
}

uint64_t PlayState::getId() const
{
	return id;
}
char PlayState::getLastTetrimino() const
{
	return lastTetrimino;
}
int PlayState::getLastRotation() const
{
	return lastRotation;
}
const Matrix& PlayState::getPlayArea() const
{
	return playArea;
}
