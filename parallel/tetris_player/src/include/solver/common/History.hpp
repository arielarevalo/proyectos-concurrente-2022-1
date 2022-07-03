// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <cstddef>
#include <queue>
#include <memory>

#include "./GameState.hpp"
#include "./PlayState.hpp"

/**
 * Work Unit
 */
class History
{
public:
	explicit History(const GameState& gameState)
			:gameState(gameState)
	{
	}

	History(const GameState& gameState, bool isStopCondition);

	bool operator==(const History& other) const;

	/**
	 * @brief Determines children of a given parent play state and calls
	 * ::findBestMoves on each of them.
	 * @param parent Parent play state currently being solved.
	 * @param parentDepth Parent solution depth.
	 * @return Whether a high score was found in children.
	 */
	std::queue<History> permutate() const;

	/**
	 *
	 * @return
	 */
	bool place() const;

	void push(const std::shared_ptr<PlayState>& playState);

	std::shared_ptr<PlayState> pop();

	bool isMaxDepth() const;

	std::shared_ptr<PlayState> getLast() const;

	size_t getSize() const;

	bool isEmpty() const;

private:
	Tetrimino::Figure getNextTetrimino() const;

	Tetrimino::Figure getLastTetrimino() const;

	const GameState& gameState;

	size_t nextRotation{ 0 };

	size_t nextColumn{ 0 };

	std::queue<std::shared_ptr<PlayState>> value{};
};

History::History(const GameState& gameState, bool isStopCondition)
		:gameState(gameState)
{
	if (isStopCondition)
	{
		while (!isMaxDepth())
		{
			push(std::make_shared<PlayState>(gameState));
		}
		push(std::make_shared<PlayState>(gameState));
	}
}

bool History::operator==(const History& other) const
{
	return (this->gameState == other.gameState)
			&& (this->value == other.value)
			&& (this->nextRotation == other.nextRotation)
			&& (this->nextColumn == other.nextColumn);
}

std::queue<History> History::permutate() const
{
	const std::shared_ptr<PlayState>& last{ getLast() };

	size_t rotations{
			Tetrimino::getTetriminoRotations(getNextTetrimino())
	};

	std::queue<History> histories{};

	for (size_t r{ 0 }; r < rotations; ++r)
	{
		for (size_t c{ 0 }; c < last->getPlayArea().cols; ++c)
		{
			History history{ *this };
			history.nextRotation = r;
			history.nextColumn = c;
			history.push(std::make_shared<PlayState>(*last));
			if(history.place()) {
				histories.push(history);
			}
		}
	}
	return histories;
}

bool History::place() const
{
	if (!isEmpty())
	{
		Tetrimino::Figure nextTetrimino{ getLastTetrimino() };
		return getLast()->place(nextTetrimino, nextRotation, nextColumn);
	}
	else
	{
		return true;
	}
}

void History::push(const std::shared_ptr<PlayState>& playState)
{
	value.push(playState);
}

Tetrimino::Figure History::getNextTetrimino() const
{
	return gameState.nextTetriminos[getSize()];
}

Tetrimino::Figure History::getLastTetrimino() const
{
	return gameState.nextTetriminos[getSize() - 1];
}

std::shared_ptr<PlayState> History::getLast() const
{
	return value.empty() ? std::make_shared<PlayState>(gameState)
						 : value.back();
}

size_t History::getSize() const
{
	return value.size();
}

bool History::isEmpty() const
{
	return value.empty();
}
std::shared_ptr<PlayState> History::pop()
{
	std::shared_ptr<PlayState> out{ value.front() };
	value.pop();
	return out;
}

bool History::isMaxDepth() const
{
	return getSize() == gameState.depth + 1;
}
