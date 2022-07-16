// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

#include "./Job.hpp"
#include "./Map.hpp"

/**
 * @brief Reads input file to game state and writes to output file with
 * history of best play state.
 */
class Filer
{
public:
	static Job toJob(const std::string& file);
	static Map toMap(const std::string& file);
	static void toFile(Map map, const std::string& file);
};
