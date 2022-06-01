// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_FACTORY_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_FACTORY_H_

#include "./play_state.h"
#include "./tetrimino.h"

/**
 * @brief Creates a new play_state structure.
 * @details Allocates memory for a new play_state struct, builds it, and returns its location
 * @param id Unique id to identify this play_state and its lineage
 * @param last_tetrimino Last tetrimino to be placed in the play area
 * @param last_rotation The rotation of the last tetrimino to be placed
 * @param play_area Pointer to a matrix representing the play area
 * @return Pointer to location of newly created play_state struct
 */
play_state *create_play_state(uint64_t id,
                              char last_tetrimino,
                              int last_rotation,
                              matrix *play_area);

/**
 * @brief Deep copies a play_state structure.
 * @details Allocates memory for a new play_state struct and assigns values identical to param
 * @param input Pointer to play_state structure to copy
 * @return Pointer to location of newly created play_state struct identical to param
 */
play_state *copy_play_state(play_state *input);

/**
 * @brief Destroys the play_state struct passed pointer.
 * @details Frees the memory at the location of the play_state and all its variables, then nulls the pointers
 * @param play_state Pointer to play_state structure to be destroyed
 */
void destroy_play_state(play_state *play_state);

/**
 * @brief Attempts to add a new tetrimino to the passed play_state.
 * @details Attempts to add a new tetrimino to the play_area of the passed play_state at the given column
 * @param play_state Pointer to play_state to update
 * @param tetrimino Tetrimino to add to the play_state
 * @param rotation Rotation of tetrimino to add
 * @param column Column of the play_area at which to attempt to add the tetrimino
 * @return An error code:
 *  0 if placement is invalid
 *  1 if placement is valid (success)
 */
int place(play_state *play_state, char tetrimino, int rotation, int column);

/**
 * @brief Obtains number of rotations for a given tetrimino.
 * @details Acts as an interface with tetrimino_factory to get rotations
 * @param tetrimino Char for tetrimino to query
 * @return Number of valid rotations for given tetrimino
 */
int get_num_rotations(char tetrimino);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_PLAY_STATE_FACTORY_H_
