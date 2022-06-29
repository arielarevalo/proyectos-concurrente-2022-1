// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <cstddef>
#include <queue>

class Permutator {
public:
    /**
	 * @brief Determines children of a given parent play state and calls
	 * ::findBestMoves on each of them.
	 * @param parent Parent play state currently being solved.
	 * @param parentDepth Parent solution depth.
	 * @return Whether a high score was found in children.
	 */
    static std::queue<History> permutate(const History& parent);
};

std::queue<History> Permutator::permutate(const History& parent)
{
    int currentDepth{ parent.size() - 1 };

    Tetrimino::Figure nextTetrimino{ gameState.nextTetriminos[currentDepth] };
    size_t rotations{ Tetrimino::getTetriminoRotations(nextTetrimino) };

    std::queue<History> histories{};

    for (size_t r{ 0 }; r < rotations; ++r)
    {
        for (size_t c{ 0 }; c < parent.getPlayArea().cols; ++c)
        {
            histories.push(History{parent, nextTetrimino, r, c});
        }
    }

    return histories;
}