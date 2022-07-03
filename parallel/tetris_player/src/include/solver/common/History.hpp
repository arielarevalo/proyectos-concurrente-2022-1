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

	bool operator==(const History& other) const;

	/**
	 * @brief Determines children of a given parent play state and calls
	 * ::findBestMoves on each of them.
	 * @param parent Parent play state currently being solved.
	 * @param parentDepth Parent solution depth.
	 * @return Whether a high score was found in children.
	 */
	std::queue<History> permutate();

	/**
	 *
	 * @return
	 */
	bool place();

	void setState(size_t nextRotation, size_t nextColumn);

	void push(const std::shared_ptr<PlayState>& playState);

	std::shared_ptr<PlayState> pop();

	const std::shared_ptr<PlayState>& getLast() const;

	size_t getSize() const;

	bool isDone() const;

	void setDone();

	bool isEmpty() const;

private:
	Tetrimino::Figure getNextTetrimino() const;

	const GameState& gameState;

	size_t nextRotation{ 0 };

	size_t nextColumn{ 0 };

	bool done{ false };

	std::queue<std::shared_ptr<PlayState>> value{};
};

bool History::operator==(const History& other) const
{
	return this->value == other.value;
}

std::queue<History> History::permutate()
{
	push(getLast());

	size_t rotations{
			Tetrimino::getTetriminoRotations(getNextTetrimino())
	};

	std::queue<History> histories{};

	for (size_t r{ 0 }; r < rotations; ++r)
	{
		for (size_t c{ 0 }; c < getLast()->getPlayArea().cols; ++c)
		{
			nextRotation = r;
			nextColumn = c;
			histories.push(*this);
		}
	}
	return histories;
}

bool History::place()
{
	/**
	 * 	nextTetrimino{ getNextTetrimino() }
	 * 	return getLast().place ( nextRot, nextCol, nextTetrimino )
	 */
	return false;
}

void History::setState(size_t nextRotation, size_t nextColumn)
{
	this->nextRotation = nextRotation;
	this->nextColumn = nextColumn;
}

void History::push(const std::shared_ptr<PlayState>& playState)
{
	value.push(playState);
}

Tetrimino::Figure History::getNextTetrimino() const
{
	return gameState.nextTetriminos[getSize()];
}

const std::shared_ptr<PlayState>& History::getLast() const
{
	return value.back();
}

size_t History::getSize() const
{
	return value.size();
}

bool History::isDone() const
{
	return done;
}
void History::setDone()
{
	done = true;
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
