// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <cstddef>
#include <queue>
#include <memory>

#include "./GameState.hpp"
#include "./PlayState.hpp"

class History {
public:
    explicit History(const GameState& gameState) : gameState(gameState) {}

	bool operator==(const History& other) const;

    const std::shared_ptr<PlayState>& back() const;

    void push(const std::shared_ptr<PlayState>& playState);

    size_t size() const;

	void setState(Tetrimino::Figure nextTetrimino,
			size_t nextRotation,
			size_t nextColumn);

	bool isDone() const;

	void setDone();

private:
    const GameState& gameState;

    Tetrimino::Figure nextTetrimino{Tetrimino::Figure::T};

    size_t nextRotation{0};

    size_t nextColumn{0};

	bool done{false};

    std::queue<std::shared_ptr<PlayState>> value{};
};

bool History::operator==(const History& other) const {
	return this->value == other.value;
}

const std::shared_ptr<PlayState>& History::back() const
{
	return value.back();
}

void History::push(const std::shared_ptr<PlayState>& playState)
{
	value.push(playState);
}

size_t History::size() const
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

void History::setState(Tetrimino::Figure nextTetrimino,
		size_t nextRotation,
		size_t nextColumn)
{
	this->nextTetrimino = nextTetrimino;
	this->nextRotation = nextRotation;
	this->nextColumn = nextColumn;
}
