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
    static constexpr char tree { 'a' };

    static constexpr char lake { 'l' };

    static constexpr char madow { '-' };
};