// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_FACTORY_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_FACTORY_H_

#include "./tetrimino.h"

/**
 * @brief Gets the number of possible rotations for a given tetrimino.
 * @details Given a tetrimino it returns the number of possible rotations depending on the shape
 * @param fig Character symbol representing a tetrimino. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @return Integer value representing the number of rotations (1, 2 or 4). If tetrimino is not valid it returns -1.
 *
 */
int get_tetrimino_num_rotations(char fig);

/**
 * @brief Gets a tetrimino ptr for the given symbol and number of rotations.
 * @details Returns a tetrimino ptr representing the given symbol tetrimino rotated clockwise based on the given num_rotations
 * @param fig Character symbol representing a tetrimino. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @param num_rotations Number of rotations (clockwise) to apply to the tetrimino. Accepted values: 1, 2, 4
 * @return tetrimino ptr. If tetrimino symbol is not valid or num_rotations is not valid for the given tetrimino, it returns NULL
 *
 */
tetrimino *get_tetrimino(char fig, int num_rotations);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_TETRIMINO_FACTORY_H_
