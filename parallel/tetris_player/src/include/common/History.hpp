// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <cstddef>
#include <queue>
#include <memory>

#include "./GameState.hpp"

class History {
public:
    History(const GameState gameState) : gameState(gameState) {}

    History(const History& history, Tetrimino::Figure nextTetrimino,
            size_t nextRotation, size_t nextColumn)
            : gameState(history.gameState),
            nextTetrimino(nextTetrimino),
            nextRotation(nextRotation),
            nextColumn(nextColumn),
            value(history.value)

    const std::shared_ptr<PlayState> back() const;

    void push(const std::shared_ptr<PlayState>);

    size_t size() const;

private:
    const GameState gameState;

    const Tetrimino::Figure nextTetrimino;

    const size_t nextRotation;

    const size_t nextColumn;

    std::queue<const std::shared_ptr<PlayState>> value{};
};
