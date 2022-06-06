// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <stdexcept>

static const int ONE_ROTATION{ 1 };
static const int TWO_ROTATIONS{ 2 };
static const int FOUR_ROTATIONS{ 4 };
static const int INVALID_ROTATIONS{ -1 };

class Tetrimino {
public:
	enum class Figures {
		I, Z, S, L, J, O, T
	};

	static const std::array<Tetrimino, TWO_ROTATIONS> I;
	static const std::array<Tetrimino, TWO_ROTATIONS> Z;
	static const std::array<Tetrimino, TWO_ROTATIONS> S;
	static const std::array<Tetrimino, FOUR_ROTATIONS> L;
	static const std::array<Tetrimino, FOUR_ROTATIONS> J;
	static const std::array<Tetrimino, ONE_ROTATION> O;
	static const std::array<Tetrimino, FOUR_ROTATIONS> T;

	static int getTetriminoRotations(Figures figure);

	static const Tetrimino& getTetrimino(size_t rotation, Figures figure);

	static std::vector<size_t> findBounds(size_t h, size_t w,
			const std::vector<std::string>& value) noexcept;

	const size_t height{ 0 };
	const size_t width{ 0 };
	const char character{ ' ' };
	const std::vector<size_t> bounds;
	const std::vector<std::string>& value;

private:
	Tetrimino(size_t h, size_t w, char c, const std::vector<std::string>& value)
	noexcept
			: height(h), width(w), character(c),
			bounds(findBounds(h, w, value)), value(value) { }
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

int Tetrimino::getTetriminoRotations(Tetrimino::Figures figure)
{
	switch (figure) {
	case Figures::I:
	case Figures::Z:
	case Figures::S:
		return TWO_ROTATIONS;
	case Figures::L:
	case Figures::J:
	case Figures::T:
		return FOUR_ROTATIONS;
	case Figures::O:
		return ONE_ROTATION;
	default:
		return INVALID_ROTATIONS;
	}
}

const Tetrimino& Tetrimino::getTetrimino(size_t rotation,
		Tetrimino::Figures figure)
{
	int maxRotations = getTetriminoRotations(figure);

	if (rotation>=maxRotations) {
		throw std::invalid_argument("rotation");
	}

	switch (figure) {
	case Figures::I:
		return I[rotation];
	case Figures::Z:
		return Z[rotation];
	case Figures::S:
		return S[rotation];
	case Figures::L:
		return L[rotation];
	case Figures::J:
		return J[rotation];
	case Figures::T:
		return T[rotation];
	case Figures::O:
		return O[rotation];
	default:
		throw std::invalid_argument("figure");
	}
}

std::vector<size_t> Tetrimino::findBounds(size_t h, size_t w,
		const std::vector<std::string>& value) noexcept
{
	std::vector<size_t> bounds(w);

	for (size_t j = 0; j<w; ++j) {
		for (size_t i = 0; i<h; ++i) {
			if (i+1==h || ('0'!=value[i][j] && '0'==value[i+1][j])) {
				bounds[j] = i;
				break;
			}
		}
	}
	return bounds;
}
