// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_H_

#include <stdint.h>
#include "./matrix.h"

/**
 * A single state of a game of Tetris.
 */
typedef struct play_state {
    uint64_t id;
    char last_tetrimino;
    int last_rotation;
    matrix *play_area;
} play_state;

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_H_
