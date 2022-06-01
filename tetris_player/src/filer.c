// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include <stdio.h>
#include "include/filer.h"
#include "include/matrix_factory.h"
#include "include/in_state_factory.h"

size_t num_of_digits(size_t num);

int is_valid_char(char tet);

int read(FILE *file, in_state **initial) {
    u_int64_t id = 0;
    if (!fscanf(file, "%lu", &id)) {
        return EXIT_FAILURE;
    }

    unsigned int depth = 0;
    if (!fscanf(file, "%u", &depth)) {
        return EXIT_FAILURE;
    }

    int num_rows = 0;
    if (!fscanf(file, "%d", &num_rows) || num_rows < 4) {
        return EXIT_FAILURE;
    }

    int num_cols = 0;
    if (!fscanf(file, "%d", &num_cols) || num_cols < 4) {
        return EXIT_FAILURE;
    }

    matrix *play_area = create_matrix(num_rows, num_cols);
    if (!play_area) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_rows; ++i) {
        if (!fscanf(file, "%s", play_area->value[i])) {
            destroy_matrix(play_area);
            return EXIT_FAILURE;
        }
    }

    size_t size_next_tetriminos = 0;

    /* (Practical) DEPTH MUST BE SMALLER THAN SIZE OF NEXT TETRIMINOS */
    if (!fscanf(file, "%zu ", &size_next_tetriminos)
        || depth >= size_next_tetriminos) {
        destroy_matrix(play_area);
        return EXIT_FAILURE;
    }

    char *next_tetriminos = calloc(size_next_tetriminos, sizeof(char));
    if (!next_tetriminos) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < size_next_tetriminos; ++i) {
        if (!fscanf(file, "%c ", next_tetriminos + i)) {
            destroy_matrix(play_area);
            free(next_tetriminos);
            return EXIT_FAILURE;
        }

        if (!is_valid_char(next_tetriminos[i])) {
            destroy_matrix(play_area);
            free(next_tetriminos);
            return EXIT_FAILURE;
        }
    }

    *initial = create_in_state(id, depth, play_area,
                              size_next_tetriminos, next_tetriminos);
    if (!*initial) {
        destroy_matrix(play_area);
        free(next_tetriminos);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int write(play_state **history, size_t size) {
    for (size_t i = 0; i < size && history[i]; ++i) {
        size_t buffer_size = BASE_FILENAME_SIZE + num_of_digits(i) + 1;
        char filename[buffer_size];

        if (!snprintf(filename, buffer_size, "./output/tetris_play_%zu.txt",
                      i)) {
            return EXIT_FAILURE;
        }

        FILE *fptr = fopen(filename, "w");

        if (!fptr) {
            fprintf(stderr, "Invalid file\n");
            return EXIT_FAILURE;
        }

        play_state *current = history[i];

        if (!fprintf(fptr, "%lu\n", current->id)) {
            return EXIT_FAILURE;
        }

        if (!fprintf(fptr, "%c\n", current->last_tetrimino)) {
            return EXIT_FAILURE;
        }

        if (!fprintf(fptr, "%d\n", current->last_rotation)) {
            return EXIT_FAILURE;
        }

        if (fprint_matrix(fptr, current->play_area)) {
            return EXIT_FAILURE;
        }

        fclose(fptr);
    }

    return EXIT_SUCCESS;
}

size_t num_of_digits(size_t num) {
    size_t n = 0;
    do {
        num = num / 10;
        ++n;
    } while (num > 0);
    return n;
}

int is_valid_char(char tet) {
    for (size_t i = 0; i < sizeof(VALID_TET_CHARS) / sizeof(char); ++i) {
        if (tet == VALID_TET_CHARS[i]) {
            return 1;
        }
    }
    return 0;
}
