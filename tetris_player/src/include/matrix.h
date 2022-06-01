// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * A matrix of characters.
 */
typedef struct matrix {
    int num_rows;
    int num_cols;
    char** value;
} matrix;

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_H_
