// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_FILER_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_FILER_H_

#include "./in_state.h"
#include "./play_state.h"

static const size_t BASE_FILENAME_SIZE = 25;
static const char VALID_TET_CHARS[8] = "IZSLJOT";

/**
 * @brief Reads a specification file into an in_state.
 * @details Builds an in_state structure at a given location from the contents of a file at a given location
 * @param input Pointer to a file to read from
 * @return
 */
int read(FILE *input, in_state **initial);

/**
 * @brief Writes all play_states into specification output files.
 * @details Builds files at a preset location based on the series of play_state memory locations passed to it
 * @param history Array of pointers to direct descendants of a given play_state
 * @param size Size of the array of pointers
 * @return 1 if successful, 0 if unsuccessful
 */
int write(play_state **history, size_t size);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_FILER_H_
