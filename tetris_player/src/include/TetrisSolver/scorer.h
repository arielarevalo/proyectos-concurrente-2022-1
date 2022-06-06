// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Adapted from Takado8's Tetris AI heuristic.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_SCORER_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_SCORER_H_

#include "./in_state.h"
#include "./matrix.h"
#include "./play_state.h"

static const int HI_SCORE = -1;

/**
 * @brief Determines the top scoring moves for the current play state.
 * @details Navigates a tree of all possible states to find the series of moves
 * that achieve the highest score possible.
 * @param initial Initial state of play, along with determinant information
 * @param current Current state of play
 * @param history Output array for the history of top scoring state
 * @param current_depth Current scoring depth
 * @return 1 if failure, 0 if successful, -1 if high score
 */
int find_best_moves(in_state *initial, play_state *current,
                    play_state **history, int current_depth);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_SCORER_H_
