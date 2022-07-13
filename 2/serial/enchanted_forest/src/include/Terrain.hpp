// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Contains shape and dimensions of a single tetrimino.
 * @details Tetriminoes are immutable. As such, functions are provided to
 * access static copies of all Tetriminoes.
 */
class Terrain
{
public:
	static const Terrain& getInstance(char key);

	// Deleted to preserve immutability
	Terrain() = delete;
	Terrain(const Terrain&) = delete;
	Terrain(const Terrain&&) = delete;
	Terrain& operator=(const Terrain&) = delete;
	Terrain& operator=(const Terrain&&) = delete;

    bool operator==(const Terrain& other) const;

	const char type;
private:
	static const Terrain arbol;
	static const Terrain lago;
	static const Terrain pradera;

	/**
	 * Constructs a Tetrimino from a full set of fields.
	 * @param type Height of the Tetrimino.
	 */
	explicit Terrain(char type)
			:type(type)
	{
	}
};

const Terrain Terrain::arbol{ 'a' };
const Terrain Terrain::lago{ 'l' };
const Terrain Terrain::pradera{ '-' };

bool Terrain::operator==(const Terrain& other) const
{
    return (Terrain == other);
};

const Terrain& Terrain::getInstance(char key)
{
	switch (key)
	{
	case 'a':
		return arbol;
	case 'l':
		return lago;
	case '-':
		return pradera;
	default:
		std::string message{ "Character does not have terrain assigned:" };
		message.push_back(key);
		throw std::invalid_argument(message);
	}
}
