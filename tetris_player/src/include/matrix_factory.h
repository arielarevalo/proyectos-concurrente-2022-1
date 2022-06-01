// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#ifndef TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_FACTORY_H_
#define TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_FACTORY_H_

#include "./matrix.h"

/**
 * @brief Creates a matrix of characters of the specified dimensions.
 * @param num_rows Number of rows to make the matrix
 * @param num_cols Number of columns to make the matrix
 * @return A pointer to a newly created matrix
 */
matrix *create_matrix(int num_rows, int num_cols);

/**
 * @brief Creates a new copy of a specified matrix of characters.
 * @param input Pointer to the matrix to copy
 * @return A pointer to a newly created copy of a the input matrix
 */
matrix *copy_matrix(matrix *input);

/**
 * @brief Destroys the specified matrix.
 * @param matrix Pointer to the matrix to destroy
 */
void destroy_matrix(matrix *input);

/**
 * @brief Prints the contents of specified matrix to a file.
 * @param fptr Pointer to the file to print to
 * @param matrix Pointer to the matrix to print
 * @return An error code:
 *  0 if fprintf is not able to print to the file
 *  1 for success
 */
int fprint_matrix(FILE *fptr, matrix *input);

/**
 * @brief Prints the contents of specified matrix to console.
 * @param matrix Pointer to the matrix to print
 * @return An error code:
 *  0 if printf is not able to print to the console
 *  1 for success
 */
int print_matrix(matrix *input);

#endif  // TAREAS_TETRIS_SOLVER_SERIAL_SRC_INCLUDE_MATRIX_FACTORY_H_
