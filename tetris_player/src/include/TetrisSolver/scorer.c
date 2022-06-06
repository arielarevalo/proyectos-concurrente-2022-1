// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include <stdio.h>
#include <string.h>
#include "include/logger.h"
#include "include/matrix_factory.h"
#include "include/play_state_factory.h"
#include "scorer.h"

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
