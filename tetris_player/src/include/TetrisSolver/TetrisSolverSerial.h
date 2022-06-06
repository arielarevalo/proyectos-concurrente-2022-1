// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <fstream>
#include <utility>
#include "Filer.h"
#include "GameState.h"

class TetrisSolverSerial {
public:
    static void play(std::ifstream& file);
private:
	static const int INITIAL_DEPTH = -1;
};

void TetrisSolverSerial::play(std::ifstream& file)
{
	GameState initial{Filer::read(file)};
	play_state *initial_ps = NULL;
	play_state **history = NULL;
	FILE *fptr = NULL;
}
