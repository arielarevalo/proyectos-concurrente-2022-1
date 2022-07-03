// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./GameState.hpp"
#include "./PlayState.hpp"

/**
 *
 */
class WorkState
{
public:
	/**
	 *
	 * @param gameState
	 */
	explicit WorkState(const GameState& gameState)
			:gameState(gameState), playState(gameState)
	{
	}

	/**
	 *
	 * @param parent
	 */
	explicit WorkState(const std::shared_ptr<WorkState> parent)
			:parent(parent), gameState(parent->gameState),
			 playState(parent->playState), depth(parent->depth + 1)
	{
	}

	bool operator==(const WorkState& other) const;

	bool place();

private:
	const std::shared_ptr<WorkState> parent{};

	const GameState& gameState;

	PlayState playState;

	size_t depth{0};

	size_t rotation{0};

	size_t column{0};
};

bool WorkState::operator==(const WorkState& other) const
{
	return (this->parent == other.parent)
			&& (this->gameState == other.gameState)
			&& (this->depth == other.depth)
			&& (this->rotation == other.rotation)
			&& (this->column == other.column);
}

bool WorkState::place()
{
	Tetrimino::Figure nextTetrimino{ gameState.nextTetriminos[depth] };
		return playState.place(nextTetrimino, rotation, column);
}

/*
bool WorkState::work()
{

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
}*/
