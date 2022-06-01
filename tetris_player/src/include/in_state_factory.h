// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "./in_state.h"

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_FACTORY_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_FACTORY_H_

in_state *create_in_state(uint64_t id,
                          unsigned int depth,
                          matrix *play_area,
                          size_t size_next_tetriminos,
                          char *next_tetriminos);

void destroy_in_state(in_state *state);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_IN_STATE_FACTORY_H_
