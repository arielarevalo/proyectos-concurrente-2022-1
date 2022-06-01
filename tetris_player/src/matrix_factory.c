// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <string.h>
#include "include/matrix_factory.h"

void **create_matrix_value(size_t row_count,
                           size_t col_count,
                           size_t element_size);

void free_matrix_value(size_t row_count, void **matrix);

matrix *create_matrix(int num_rows, int num_cols) {
    matrix *output = malloc(sizeof(matrix));
    if (!output) {
        return NULL;
    }

    output->num_rows = num_rows;
    output->num_cols = num_cols;
    output->value = (char **) create_matrix_value(output->num_rows,
                                                  output->num_cols,
                                                  sizeof(char));

    if (!output->value) {
        free(output);
        return NULL;
    }

    return output;
}

matrix *copy_matrix(matrix *input) {
    matrix *output = create_matrix(input->num_rows, input->num_cols);
    if (!output) {
        return NULL;
    }

    for (size_t i = 0; i < (size_t) output->num_rows; ++i) {
        strncpy(output->value[i], input->value[i], output->num_cols);
    }

    return output;
}

void destroy_matrix(matrix *input) {
    free_matrix_value(input->num_rows, (void **) input->value);
    free(input);
}

void **create_matrix_value(size_t row_count,
                           size_t col_count,
                           size_t element_size) {
    void **output = (void **) calloc(row_count, sizeof(void *));
    if (!output) {
        return NULL;
    }

    for (size_t row = 0; row < row_count; ++row) {
        if (!(output[row] = calloc(col_count + 1, element_size))) {
            free_matrix_value(row_count, output);
            return NULL;
        }
    }

    return output;
}

void free_matrix_value(size_t row_count, void **input) {
    if (input) {
        for (size_t row = 0; row < row_count; ++row) {
            free(input[row]);
        }
        free(input);
    }
}

int fprint_matrix(FILE *fptr, matrix *input) {
    if (!fprintf(fptr, "%d\n", input->num_rows)) {
        return EXIT_FAILURE;
    }

    if (!fprintf(fptr, "%d\n", input->num_cols)) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < input->num_rows; ++i) {
        if (!fprintf(fptr, "%s\n", input->value[i])) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


int print_matrix(matrix *input) {
    for (int i = 0; i < input->num_rows; ++i) {
        if (!printf("\t%s\n", input->value[i])) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

