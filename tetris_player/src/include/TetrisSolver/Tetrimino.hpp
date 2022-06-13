// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <stdexcept>

static const size_t ONE_ROTATION{ 1 };
static const size_t TWO_ROTATIONS{ 2 };
static const size_t FOUR_ROTATIONS{ 4 };

/**
 * @details Tetrimino Class
 */
class Tetrimino
{
public:
	enum class Figure
	{
		I, Z, S, L, J, O, T
	};

	static const std::array<Tetrimino, TWO_ROTATIONS> I;
	static const std::array<Tetrimino, TWO_ROTATIONS> Z;
	static const std::array<Tetrimino, TWO_ROTATIONS> S;
	static const std::array<Tetrimino, FOUR_ROTATIONS> L;
	static const std::array<Tetrimino, FOUR_ROTATIONS> J;
	static const std::array<Tetrimino, ONE_ROTATION> O;
	static const std::array<Tetrimino, FOUR_ROTATIONS> T;

	Tetrimino() = delete;
	Tetrimino(const Tetrimino&) = delete;
	Tetrimino& operator=(const Tetrimino&) = delete;
	Tetrimino& operator=(Tetrimino&&) = delete;

	/**
	 * @brief Gets the number of possible rotations for a given Tetrimino figure.
	 * @details Given a Tetrimino figure it returns the number of possible rotations depending on the shape
	 * @param figure Character symbol representing a Tetrimino figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
	 * @return Integer value representing the number of rotations (1, 2 or 4). If figure is not valid it returns -1.
	 *
	 */
	static size_t getTetriminoRotations(Figure figure);

	/**
	 * @brief Gets a Tetrimino for the given symbol and number of rotations.
	 * @details Returns a Tetrimino representing the given symbol figure rotated clockwise based on the given num_rotations
	 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
	 * @param num_rotations Number of rotations (clockwise) to apply to the figure. Accepted values: 1, 2, 4
	 * @return Tetrimino. If figure symbol is not valid or num_rotations is not valid for the given figure, it returns NULL
	 *
	 */
	static const Tetrimino& getTetrimino(size_t rotation, Figure figure);

	/**
	 * @brief Gets a Tetrimino bounds.
	 * @details Determine the tetrimino dimensions
	 * @param h = height: size_t
	 * @param w = width: size_t
	 * @param tetriminio values: vector<string>
	 * @return vector<size_t> findBounds
	 *
	 */
	static std::vector<size_t> findBounds(size_t h, size_t w,
			const std::vector<std::string>& value);

	/**
	 * @brief Gets the figure enum.
	 * @details Return a figure enumeration from a character c
	 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
	 * @return Figure enum
	 *
	 */
	static Figure charToEnum(char c);

	const size_t height{ 0 };
	const size_t width{ 0 };
	const char character{ ' ' };
	const std::vector<size_t> bounds;
	const std::vector<std::string> value;

private:
	Tetrimino(Tetrimino&&) noexcept = default;
	Tetrimino(size_t h, size_t w, char c,
			const std::vector<std::string>& value) noexcept
			:height(h), width(w), character(c),
			 bounds(findBounds(h, w, value)), value(value)
	{
	}
};

const std::array<Tetrimino, TWO_ROTATIONS> Tetrimino::I{
		Tetrimino(1, 4, 'I', { "1111" }),
		Tetrimino(4, 1, 'I', { "1", "1", "1", "1" })
};

const std::array<Tetrimino, TWO_ROTATIONS> Tetrimino::Z{
		Tetrimino(2, 3, 'Z', { "220", "022" }),
		Tetrimino(3, 2, 'Z', { "02", "22", "20" })
};

const std::array<Tetrimino, TWO_ROTATIONS> Tetrimino::S{
		Tetrimino({ 2, 3, 'S', { "033", "330" }}),
		Tetrimino({ 3, 2, 'S', { "30", "33", "03" }})
};

const std::array<Tetrimino, FOUR_ROTATIONS> Tetrimino::L{
		Tetrimino({ 2, 3, 'L', { "004", "444" }}),
		Tetrimino({ 3, 2, 'L', { "40", "40", "44" }}),
		Tetrimino({ 2, 3, 'L', { "444", "400" }}),
		Tetrimino({ 3, 2, 'L', { "44", "04", "04" }})
};

const std::array<Tetrimino, FOUR_ROTATIONS> Tetrimino::J{
		Tetrimino({ 2, 3, 'J', { "500", "555" }}),
		Tetrimino({ 3, 2, 'J', { "55", "50", "50" }}),
		Tetrimino({ 2, 3, 'J', { "555", "005" }}),
		Tetrimino({ 3, 2, 'J', { "05", "05", "55" }})
};

const std::array<Tetrimino, ONE_ROTATION> Tetrimino::O{
		Tetrimino({ 2, 2, 'O', { "66", "66" }})
};
const std::array<Tetrimino, FOUR_ROTATIONS> Tetrimino::T{
		Tetrimino({ 2, 3, 'T', { "070", "777" }}),
		Tetrimino({ 3, 2, 'T', { "70", "77", "70" }}),
		Tetrimino({ 2, 3, 'T', { "777", "070" }}),
		Tetrimino({ 3, 2, 'T', { "07", "77", "07" }})
};

/**
 * @brief Gets the number of possible rotations for a given Tetrimino figure.
 * @details Given a Tetrimino figure it returns the number of possible rotations depending on the shape
 * @param figure Character symbol representing a Tetrimino figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @return Integer value representing the number of rotations (1, 2 or 4). If figure is not valid it returns -1.
 *
 */
size_t Tetrimino::getTetriminoRotations(Tetrimino::Figure figure)
{
	switch (figure)
	{
	case Figure::I:
	case Figure::Z:
	case Figure::S:
		return TWO_ROTATIONS;
	case Figure::L:
	case Figure::J:
	case Figure::T:
		return FOUR_ROTATIONS;
	case Figure::O:
		return ONE_ROTATION;
	default:
		throw std::invalid_argument("Tetrimino figure does not exist.");
	}
}

/**
 * @brief Gets a Tetrimino for the given symbol and number of rotations.
 * @details Returns a Tetrimino representing the given symbol figure rotated clockwise based on the given num_rotations
 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @param num_rotations Number of rotations (clockwise) to apply to the figure. Accepted values: 1, 2, 4
 * @return Tetrimino. If figure symbol is not valid or num_rotations is not valid for the given figure, it returns NULL
 *
 */
const Tetrimino& Tetrimino::getTetrimino(size_t rotation,
		Tetrimino::Figure figure)
{
	if (getTetriminoRotations(figure) <= rotation)
	{
		throw std::out_of_range("Rotation is out of range for figure:" +
				std::to_string(rotation));
	}

	switch (figure)
	{
	case Figure::I:
		return I[rotation];
	case Figure::Z:
		return Z[rotation];
	case Figure::S:
		return S[rotation];
	case Figure::L:
		return L[rotation];
	case Figure::J:
		return J[rotation];
	case Figure::T:
		return T[rotation];
	case Figure::O:
		return O[rotation];
	default:
		throw std::invalid_argument("Figure does not have rotations assigned.");
	}
}

/**
 * @brief Gets a Tetrimino bounds.
 * @details Determine the tetrimino dimensions
 * @param h = height: size_t
 * @param w = width: size_t
 * @param tetriminio values: vector<string>
 * @return vector<size_t> findBounds
 *
 */
std::vector<size_t> Tetrimino::findBounds(size_t h, size_t w,
		const std::vector<std::string>& value)
{
	std::vector<size_t> bounds(w);

	for (size_t j = 0; j < w; ++j)
	{
		for (size_t i = 0; i < h; ++i)
		{
			if (i + 1 == h || ('0' != value[i][j] && '0' == value[i + 1][j]))
			{
				bounds[j] = i;
				break;
			}
		}
	}
	return bounds;
}

/**
 * @brief Gets the figure enum.
 * @details Return a figure enumeration from a character c
 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @return Figure enum
 *
 */
Tetrimino::Figure Tetrimino::charToEnum(char c)
{
	switch (c)
	{
	case 'I':
		return Figure::I;
	case 'Z':
		return Figure::Z;
	case 'S':
		return Figure::S;
	case 'L':
		return Figure::L;
	case 'J':
		return Figure::J;
	case 'T':
		return Figure::T;
	case 'O':
		return Figure::O;
	default:
		std::string message{ "Character does not have figure assigned:" };
		message.push_back(c);
		throw std::invalid_argument(message);
	}
}
