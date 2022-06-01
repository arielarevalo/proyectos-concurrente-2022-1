// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "include/in_state_factory.h"
#include "include/matrix_factory.h"

in_state *create_in_state(uint64_t id,
                          unsigned int depth,
                          matrix *play_area,
                          size_t size_next_tetriminos,
                          char *next_tetriminos) {
    in_state *output = malloc(sizeof(in_state));
    if (!output) {
        return NULL;
    }

    *output = (in_state) {id, depth, play_area,
                          size_next_tetriminos,
                          next_tetriminos};

    return output;
}

void destroy_in_state(in_state *state) {
    destroy_matrix(state->play_area);
    free(state->next_tetriminos);
    free(state);
}
