// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief A matrix of characters.
 */
class Matrix {
public:
	/**
	 * @brief Constructs a matrix of characters of the specified dimensions.
	 * @param rows Number of rows.
	 * @param cols Number of columns.
	 */
	Matrix(size_t rows, size_t cols)
			:rows(rows), cols(cols),
			 value(std::vector<std::string>(rows, std::string(cols, '0'))) { }

	/**
	 * @brief Prints the matrix contents to console.
	 */
	void print() const;

	/**
	 * @brief Prints the matrix contents to a file.
	 * @param file File to print to.
	 */
	void print(std::ofstream& file) const;

	/**
	 * @brief Gets matrix row at passed value.
	 * @param i Row to get.
	 * @return Row of character matrix.
	 */
	std::string& operator[](size_t i);

	/**
	 * @brief Gets constant matrix row at passed value.
	 * @param i Row to get.
	 * @return Constant row of character matrix.
	 */
	const std::string& operator[](size_t i) const;

	const size_t rows;
	const size_t cols;
private:
	std::vector<std::string> value;
};

void Matrix::print() const
{
	for (const std::string& s : value) {
		std::cout << "\t" << s << std::endl;
	}
}

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
