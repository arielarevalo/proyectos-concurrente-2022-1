// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_H_

/**
 * A single Tetris figure's character matrix representation.
 */
typedef struct tetrimino {
    unsigned int height;
    unsigned int width;
    char** value;
} tetrimino;

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_H_
