// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <deque>
#include <memory>
#include <utility>

#include "./GameState.hpp"
#include "./PlayState.hpp"

using Position = std::pair<size_t, size_t>; // { rot, col }

/**
 *
 */
class WorkState
{
public:
	explicit WorkState(const GameState& gameState, Position pos)
			:gameState(gameState), pos(std::move(pos))
	{
	}

	explicit WorkState(const GameState& gameState)
			:gameState(gameState)
	{
	}

	bool operator==(const WorkState& other) const;

	std::deque<PlayState> work();

private:
	static constexpr Position first{ 0, 0 };

	bool process(PlayState& current, size_t currentDepth,
			size_t rotation, size_t column);

	bool explore(const PlayState& parent, size_t parentDepth);

	bool compare(const PlayState& current);

	Tetrimino::Figure getTetrimino(size_t depth) const;

	const GameState& gameState;

	std::deque<PlayState> history{};

	Position pos{ first };
};

bool WorkState::operator==(const WorkState& other) const
{
	return (this->gameState == other.gameState)
			&& (this->pos == other.pos);
}

std::deque<PlayState> WorkState::work()
{
	PlayState initial{ gameState };
	process(initial, 0, pos.first, pos.second);
	return history;
}

bool WorkState::process(PlayState& current, size_t currentDepth,
		size_t rotation, size_t column)
{
	bool isHighScore{false};
	if(current.place(getTetrimino(currentDepth), rotation, column)){
		if(currentDepth < gameState.depth) {
			isHighScore = explore(current, currentDepth);
		} else {
			isHighScore = compare(current);
		}

		if (isHighScore)
		{
			history.push_front(current);
		}
	}
	return isHighScore;
}

bool WorkState::explore(const PlayState& parent, size_t parentDepth)
{
	bool isHighScore{ false };
	size_t currentDepth{ ++parentDepth };

	Tetrimino::Figure nextTetrimino{ getTetrimino(currentDepth) };
	size_t rotations{ Tetrimino::getTetriminoRotations(nextTetrimino) };

	for (size_t r{ 0 }; r < rotations; ++r)
	{
		for (size_t c{ 0 }; c < parent.getPlayArea().cols; ++c)
		{
			PlayState temp{ parent };
			if (process(temp, currentDepth, r, c))
			{
				isHighScore = true;
			}
		}
	}
	return isHighScore;
}

bool WorkState::compare(const PlayState& current)
{
	if (history.empty() || current > history.back())
	{
		while (!history.empty())
		{
			history.pop_back();
		}
		return true;
	}
	return false;
}

Tetrimino::Figure WorkState::getTetrimino(size_t depth) const
{
	return gameState.nextTetriminos[depth];
}