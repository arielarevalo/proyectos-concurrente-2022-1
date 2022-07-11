// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstdint>
#include <utility>
#include "./GameState.hpp"
#include "./Matrix.hpp"
#include "./Tetrimino.hpp"

/**
 * @brief A single state of a play of Tetris.
 */
class PlayState
{
public:
	/**
	 * Constructs a play state from a given game state.
	 * @param gameState Game state to construct a play state from.
	 */
	explicit PlayState(const GameState& gameState)
			:id(gameState.id),
			 playArea(gameState.playArea)
	{
	}

	bool operator<(const PlayState& other) const;

	bool operator>(const PlayState& other) const;

	/**
	 * @brief Attempts to place a new Tetrimino at a given column.
	 * @param figure Tetrimino::Figure to place.
	 * @param rotation Rotation number of the figure to place.
	 * @param column Column to attempt to place to.
	 * @return Whether place was successful.
	 */
	bool place(Tetrimino::Figure figure, size_t rotation, size_t column);

	uint64_t getId() const;

	char getLastTetrimino() const;

	size_t getLastRotation() const;

	const Matrix& getPlayArea() const;

	unsigned int getScore();

private:
	static const char INITIAL_TETRIMINO{ static_cast<char>(32) };  // Whitespace
	static const size_t INITIAL_ROTATION{ 0 };

	/**
	 * @brief Scores play area based on its layout.
	 * @return Score of play area.
	 */
	void calcScore();

	const uint64_t id;
	char lastTetrimino{ INITIAL_TETRIMINO };
	size_t lastRotation{ INITIAL_ROTATION };
	Matrix playArea;
	unsigned int score{ 0 };
};

bool PlayState::operator<(const PlayState& other) const
{
	return this->score < other.score;
}

bool PlayState::operator>(const PlayState& other) const
{
	return !(*this < other);
}

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

	/* With starting position at each column based on bounds, finalize at each
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

	calcScore();

	return true;
}

void PlayState::calcScore()
{
	unsigned int s{ 0 };

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
			s += sum * i * i;
		}
	}

	this->score = s;
}

uint64_t PlayState::getId() const
{
	return id;
}

char PlayState::getLastTetrimino() const
{
	return lastTetrimino;
}

size_t PlayState::getLastRotation() const
{
	return lastRotation;
}

const Matrix& PlayState::getPlayArea() const
{
	return playArea;
}
unsigned int PlayState::getScore()
{
	if (0 == this->score)
	{
		calcScore();
	}

	return this->score;
}
