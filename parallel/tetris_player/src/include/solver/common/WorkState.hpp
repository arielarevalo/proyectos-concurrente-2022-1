// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>
#include <queue>
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
	static WorkState getNext(WorkState const& original);

	static const WorkState stopCondition;

	WorkState(const std::shared_ptr<WorkState> parent, Position pos)
			:WorkState(parent, parent->gameState, parent->playState, pos)
	{
	}

	explicit WorkState(const std::shared_ptr<WorkState> parent)
			:WorkState(parent, first)
	{
	}

	explicit WorkState(const GameState& gameState, Position pos)
			:gameState(gameState), playState(gameState), pos(pos)
	{
	}

	explicit WorkState(const GameState& gameState)
			:WorkState(gameState, PlayState{ gameState })
	{
	}

	bool operator==(const WorkState& other) const;

	bool operator<(const WorkState& other) const;

	bool operator>(const WorkState& other) const;

	bool place();

	bool isMaxDepth() const;

	std::queue<PlayState> getPlayStates() const;

private:
	static constexpr Position first{ 0, 0 };

	WorkState(const std::shared_ptr<WorkState> parent,
			const GameState& gameState, const PlayState& playState,
			Position pos)
			:parent(parent), gameState(gameState),
			 playState(playState), depth(parent->depth + 1),
			 pos(pos)
	{
	}

	WorkState(const GameState& gameState, const PlayState& playState)
			:gameState(gameState), playState(playState)
	{
	}

	Position getNextPos() const;

	Tetrimino::Figure getTetrimino() const;

	const std::shared_ptr<WorkState> parent{};

	const GameState& gameState;

	PlayState playState;

	size_t depth{ 0 };

	Position pos{ first };
};

const WorkState WorkState::stopCondition{ GameState{}};

WorkState WorkState::getNext(WorkState const& original)
{
	if (original.parent)
	{
		Position nextPos{ original.getNextPos() };
		if (nextPos != first)
		{
			return { original.parent, nextPos };
		}
		else
		{
			return getNext(*original.parent);
		}
	}
	else
	{
		return stopCondition;
	}
}


bool WorkState::operator==(const WorkState& other) const
{
	return (this->parent == other.parent)
			&& (this->gameState == other.gameState)
			&& (this->depth == other.depth)
			&& (this->pos == other.pos);
}

bool WorkState::operator<(const WorkState& other) const
{
	return this->playState < other.playState;
}

bool WorkState::operator>(const WorkState& other) const
{
	return *this < other;
}

bool WorkState::place()
{
	return playState.place(getTetrimino(), pos.first, pos.second);
}

bool WorkState::isMaxDepth() const
{
	return depth == gameState.depth;
}

std::queue<PlayState> WorkState::getPlayStates() const
{
	std::queue<PlayState> out{};
	out.push(playState);

	std::shared_ptr<WorkState> ptr{ parent };
	while (ptr)
	{
		out.push(ptr->playState);
		ptr = ptr->parent;
	}
	return out;
}

Position WorkState::getNextPos() const
{
	size_t rotations{ Tetrimino::getTetriminoRotations(getTetrimino()) };
	if (pos.second < playState.getPlayArea().cols - 1)
	{
		return { pos.first, pos.second + 1 };
	}
	else if (pos.first < rotations - 1)
	{
		return { pos.first + 1, 0 };
	}
	else
	{
		return first;
	}
}

Tetrimino::Figure WorkState::getTetrimino() const
{
	return gameState.nextTetriminos[depth];
}
