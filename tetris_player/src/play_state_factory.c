// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "include/matrix_factory.h"
#include "include/play_state_factory.h"
#include "include/tetrimino_factory.h"

void find_bounds(tetrimino *new_tetrimino, unsigned int bounds[]);

play_state *create_play_state(uint64_t id,
                              char last_tetrimino,
                              int last_rotation,
                              matrix *play_area) {
    play_state *output = malloc(sizeof(play_state));
    if (!output) {
        return NULL;
    }

    *output = (play_state) {id, last_tetrimino, last_rotation, play_area};

    return output;
}

play_state *copy_play_state(play_state *input) {
    play_state *output = malloc(sizeof(play_state));
    if (!output) {
        return NULL;
    }

    *output = (play_state) {input->id,
                            input->last_tetrimino,
                            input->last_rotation,
                            copy_matrix(input->play_area)};
    if (!output->play_area) {
        free(output);
        return NULL;
    }

    return output;
}

void destroy_play_state(play_state *state) {
    destroy_matrix(state->play_area);
    free(state);
}

int place(play_state *state,
          char new_tetrimino,
          int rotation,
          int column) {
    matrix *play_area = state->play_area;
    tetrimino *tet_to_add = get_tetrimino(new_tetrimino, rotation);

    /* Confirm tetrimino fits board */
    if (tet_to_add->width > (unsigned int) play_area->num_cols - column) {
        return EXIT_FAILURE;
    }

    unsigned int bounds[tet_to_add->width];
    find_bounds(tet_to_add, bounds);

    /* Confirm space is open for tet at the top */
    for (unsigned int j = 0; j < tet_to_add->width; ++j) {
        for (unsigned int i = 0; i <= bounds[j]; ++i) {
            if ('0' != play_area->value[i][j + column]) {
                return EXIT_FAILURE;
            }
        }
    }

    /* With starting position at each column based on bounds, check at each
     * column if next downward cell is non-zero, if so, stop. At this point
     * "d" is the displacement of the tetrimino from the initial position. */
    unsigned int d;
    int cont = 1;
    for (d = 0;; ++d) {
        for (unsigned int j = 0; j < tet_to_add->width; ++j) {
            if (bounds[j] + d + 1 >= (unsigned int) play_area->num_rows ||
                '0' != play_area->value[bounds[j] + d + 1][j + column]) {
                cont = 0;
            }
        }

        if (!cont) {  // Skip last ++d
            break;
        }
    }

    /* Paint the play_area with the nominal values of the tetrimino shape
     * plus some displacement "d". */
    for (unsigned int i = 0; i < tet_to_add->height; ++i) {
        for (unsigned int j = 0; j < tet_to_add->width; ++j) {
            char current_value = tet_to_add->value[i][j];
            if ('0' != current_value) {
                play_area->value[i + d][j + column] = current_value;
            }
        }
    }

    state->last_tetrimino = new_tetrimino;
    state->last_rotation = rotation;

    return EXIT_SUCCESS;
}

int get_num_rotations(char tetrimino) {
    return get_tetrimino_num_rotations(tetrimino);
}

void find_bounds(tetrimino *new_tetrimino, unsigned int bounds[]) {
    for (unsigned int j = 0; j < new_tetrimino->width; ++j) {
        for (unsigned int i = 0; i < new_tetrimino->height; ++i) {
            if (i + 1 == new_tetrimino->height ||
                ('0' != new_tetrimino->value[i][j] &&
                 '0' == new_tetrimino->value[i + 1][j])) {
                bounds[j] = i;
                break;
            }
        }
    }
}
