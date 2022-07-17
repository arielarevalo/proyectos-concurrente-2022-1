// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Point = std::pair<int, int>;

/**
 * @brief A matrix of characters.
 */
template<typename T>
class Matrix
{
public:

	using Row = std::vector<T>;
	using Value = std::vector<Row>;

	/**
	 * @brief Constructs a matrix of characters of the specified dimensions.
	 * @param rows Number of rows.
	 * @param cols Number of columns.
	 */
	Matrix(size_t rows, size_t cols)
			:rows(rows), cols(cols),
			 value(rows, Row(cols))
	{
	}

	/**
	 * @brief Gets matrix row at passed value.
	 * @param i Row to get.
	 * @return Row of character matrix.
	 */
	T& operator[](const Point& p);

	/**
	 * @brief Gets constant matrix row at passed value.
	 * @param i Row to get.
	 * @return Constant row of character matrix.
	 */
	const T& operator[](const Point& p) const;

	const size_t rows;
	const size_t cols;
private:
	Value value;
};

template<typename T>
T& Matrix<T>::operator[](const Point& p)
{
	return value[p.first][p.second];
}

template<typename T>
const T& Matrix<T>::operator[](const Point& p) const
{
	return value[p.first][p.second];
}
