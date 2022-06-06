// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include <stdio.h>
#include "include/logger.h"
#include "include/filer.h"
#include "scorer.h"
#include "include/in_state_factory.h"
#include "include/play_state_factory.h"
#include "include/matrix_factory.h"

static const char INITIAL_TETRIMINO = (char) 32;  // Whitespace
static const int INITIAL_ROTATION = -1;  // Invalid
static const int INITIAL_DEPTH = -1;

static in_state *initial = NULL;
static play_state *initial_ps = NULL;
static play_state **history = NULL;
static FILE *fptr = NULL;

void finalize();

int main(int argc, char **args) {
    if (argc == 2) {
        fptr = fopen(args[1], "r");
        if (!fptr) {
            log_error("Failed to open input file.");
            return EXIT_FAILURE;
        }
        log_info("Successfully opened input file.");
    } else {
        log_error("Failed: Number of input args is not two.");
        return EXIT_FAILURE;
    }

    if (read(fptr, &initial)) {
        log_error("Failed to read input file.");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    log_info("Successfully read input file.");

    fclose(fptr);
    log_info("Successfully closed input file.");

    initial_ps = create_play_state(initial->id,
                                   INITIAL_TETRIMINO,
                                   INITIAL_ROTATION,
                                   copy_matrix(initial->play_area));
    if (!initial_ps) {
        log_info("Failed to build initial play state.");
        finalize();
        return EXIT_FAILURE;
    }
    log_info("Successfully built initial play state.");

    history = calloc(initial->depth + 1, sizeof(play_state *));
    if (!history) {
        log_error("Failed to build history array.");
        finalize();
        return EXIT_FAILURE;
    }
    log_info("Successfully built history array.");

    int code = find_best_moves(initial, initial_ps, history, INITIAL_DEPTH);
    if (code > HI_SCORE) {
        if(code > EXIT_SUCCESS) {
            log_error("Failed to find best moves.");
        }
        log_error("Failed to find valid moves to the desired depth.");
        finalize();
        return EXIT_FAILURE;
    }
    log_info("Successfully found best moves.");

    if (write(history, initial->depth + 1)) {
        log_error("Failed to write output files.");
        finalize();
        return EXIT_FAILURE;
    }
    log_info("Successfully wrote output files.");

    log_info("Finalizing...");
    finalize();
    return EXIT_SUCCESS;
}

void finalize() {
    if (history) {
        for (size_t i = 0; i < initial->depth + 1; ++i) {
            if (history[i]) {
                destroy_play_state(history[i]);
            }
        }
        log_info("Successfully destroyed history.");
    }

    if (initial_ps) {
        destroy_play_state(initial_ps);
        log_info("Successfully destroyed initial play state.");
    }

    if (initial) {
        destroy_in_state(initial);
        log_info("Successfully destroyed initial.");
    }
}
