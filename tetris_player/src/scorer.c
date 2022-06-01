// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include <stdio.h>
#include <string.h>
#include "include/logger.h"
#include "include/matrix_factory.h"
#include "include/play_state_factory.h"
#include "include/scorer.h"

static const double C_HEIGHT = -0.798752914564018;  // a
static const double C_LINES = 0.522287506868767;  // b
static const double C_HOLES = -0.24921408023878;  // c
static const double C_BUMPY = -0.164626498034284;  // d

int find_best_child(in_state *initial, int parent_depth,
                    play_state *parent, play_state **history);

int compare_to_best(int final_depth, play_state *current, play_state **history);

void clear_history(play_state **history, int final_depth);

unsigned int score(matrix *play_area);

int get_complete_lines(matrix *play_area);

int get_height(matrix *play_area, int complete_lines);

int get_holes(matrix *play_area, int complete_lines);

int get_bumpiness(matrix *play_area, int complete_lines);

int find_best_moves(in_state *initial, play_state *current,
                    play_state **history, int current_depth) {
    int final_depth = (int) initial->depth;

    if (current_depth < final_depth) {
        int child_hs = find_best_child(initial, current_depth,
                                       current, history);
        if (current_depth > -1 && HI_SCORE == child_hs) {
            history[current_depth] = copy_play_state(current);
        }
        return child_hs;
    } else if (current_depth == final_depth) {
        return compare_to_best(final_depth, current, history);
    } else {
        return EXIT_FAILURE;
    }
}

int find_best_child(in_state *initial, int parent_depth,
                    play_state *parent, play_state **history) {
    int hs = EXIT_SUCCESS;
    int current_depth = parent_depth + 1;
    char next_tetrimino = initial->next_tetriminos[current_depth];
    int rotations = get_num_rotations(next_tetrimino);

    for (int r = 0; r < rotations; ++r) {
        for (int j = 0; j < parent->play_area->num_cols; ++j) {
            int child_hs = EXIT_SUCCESS;

            play_state *temp = copy_play_state(parent);
            if (!temp) {
                return EXIT_FAILURE;
            }

            if (!place(temp, next_tetrimino, r, j)) {
                child_hs = find_best_moves(initial, temp,
                                           history, current_depth);
            }
            destroy_play_state(temp);

            if (HI_SCORE == child_hs) {
                hs = child_hs;
            }
        }
    }
    return hs;
}

int compare_to_best(int final_depth, play_state *current,
                    play_state **history) {
    if (!history[final_depth]) {
        history[final_depth] = copy_play_state(current);
        return HI_SCORE;
    } else {
        int current_score = score(current->play_area);
        int top_score = score(history[final_depth]->play_area);

        if (current_score > top_score) {
            log_info("New High Score:");
            printf("\t%u\n", current_score);
            print_matrix(current->play_area);
            log_info("Old High Score:");
            printf("\t%u\n", top_score);
            print_matrix(history[final_depth]->play_area);

            clear_history(history, final_depth);
            history[final_depth] = copy_play_state(current);
            return HI_SCORE;
        }
    }
    return EXIT_SUCCESS;
}

void clear_history(play_state **history, int final_depth) {
    for (int i = 0; i < final_depth + 1; ++i) {
        if (history[i]) {
            free(history[i]);
        }
        history[i] = NULL;
    }
}

unsigned int score(matrix *play_area) {
    int complete_lines = get_complete_lines(play_area);
    int height = get_height(play_area, complete_lines);
    int holes = get_holes(play_area, complete_lines);
    int bumpiness = get_bumpiness(play_area, complete_lines);

    return height * C_HEIGHT +
           complete_lines * C_LINES +
           holes * C_HOLES +
           bumpiness * C_BUMPY;
}

int get_complete_lines(matrix *play_area) {
    int i;  // Incomplete lines
    int cont = 1;
    for (i = play_area->num_rows; i > 0; --i) {
        for (int j = 0; j < play_area->num_cols && cont; ++j) {
            if ('0' == play_area->value[i - 1][j]) {
                cont = 0;
            }
        }

        if (!cont) {  // Skip last --i
            break;
        }
    }

    return play_area->num_rows - i;
}

int get_height(matrix *play_area, int complete_lines) {
    int height_sum = 0;
    int area_height = play_area->num_rows;
    int end_height = area_height - complete_lines;

    for (int j = 0; j < play_area->num_cols; ++j) {
        int highest_cell = end_height;

        for (int i = 0; i < end_height; ++i) {
            char current = play_area->value[i][j];

            if ('0' != current) {
                highest_cell = i;
                break;
            }
        }
        height_sum += area_height - highest_cell;
    }

    return height_sum;
}

// Takado8's code defines a hole as any empty space under a filled cell.
int get_holes(matrix *play_area, int complete_lines) {
    int holes = 0;
    int start_count = 0;

    for (int j = 0; j < play_area->num_cols; ++j) {
        for (int i = 0; i < play_area->num_rows - complete_lines; ++i) {
            char current = play_area->value[i][j];

            if ('0' != current) {
                start_count = 1;
            }
            if (start_count && '0' == current) {
                ++holes;
            }
        }
        start_count = 0;
    }

    return holes;
}

int get_bumpiness(matrix *play_area, int complete_lines) {
    int bumpiness = 0;
    int end_height = play_area->num_rows - complete_lines;

    // Cannot compare column 0 to the anything
    int prev_highest_cell = end_height;
    for (int i = 0; i < end_height; ++i) {
        char current = play_area->value[i][0];

        if ('0' != current) {
            prev_highest_cell = i;
            break;
        }
    }

    // Compare to the left
    for (int j = 1; j < play_area->num_cols; ++j) {
        int highest_cell = end_height;

        for (int i = 0; i < end_height; ++i) {
            char current = play_area->value[i][j];

            if ('0' != current) {
                highest_cell = i;
                break;
            }
        }

        // Sign is irrelevant
        int diff = highest_cell - prev_highest_cell;
        prev_highest_cell = highest_cell;

        bumpiness += diff > 0 ? diff : -diff;
    }

    return bumpiness;
}
