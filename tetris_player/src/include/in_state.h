// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_H_

#include <stdint.h>
#include "./matrix.h"

/**
 * A set of initial state information obtained from the input file.
 */
typedef struct in_state {
    uint64_t id;
    unsigned int depth;
    matrix *play_area;
    size_t size_next_tetriminos;
    char *next_tetriminos;
} in_state;

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_H_
