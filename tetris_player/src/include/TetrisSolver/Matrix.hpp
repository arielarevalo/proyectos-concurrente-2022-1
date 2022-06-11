// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * A matrix of characters.
 */
class Matrix {
public:
	/**
	 * @brief Creates a matrix of characters of the specified dimensions.
	 * @param rows Number of rows to make the matrix
	 * @param cols Number of columns to make the matrix
	 * @return A pointer to a newly created matrix
	 */
	Matrix(size_t rows, size_t cols)
			:rows(rows), cols(cols),
			 value(std::vector<std::string>(rows, std::string(cols, '0'))) { }

	/**
	 * @brief Prints the contents of specified matrix to console.
	 * @param matrix Pointer to the matrix to print
	 */
	void print() const;

	/**
	 * @brief Prints the contents of specified matrix to a file.
	 * @param fptr Pointer to the file to print to
	 * @param matrix Pointer to the matrix to print
	 */
	void print(std::ofstream& file) const;

	std::string& operator[](size_t i);

	const std::string& operator[](size_t i) const;

	const size_t rows;
	const size_t cols;
private:
	std::vector<std::string> value;
};

/**
 * @brief Prints the contents of specified matrix to console.
 * @param matrix Pointer to the matrix to print
 */
void Matrix::print() const
{
	for (const std::string& s : value) {
		std::cout << "\t" << s << std::endl;
	}
}

/**
 * @brief Prints the contents of specified matrix to a file.
 * @param fptr Pointer to the file to print to
 * @param matrix Pointer to the matrix to print
 */
void Matrix::print(std::ofstream& file) const
{
	file << rows << std::endl;
	file << cols << std::endl;
	for (const std::string& s : value) {
		file << s << std::endl;
	}
}

std::string& Matrix::operator[](size_t i)
{
	return value[i];
}

const std::string& Matrix::operator[](size_t i) const
{
	return value[i];
}
