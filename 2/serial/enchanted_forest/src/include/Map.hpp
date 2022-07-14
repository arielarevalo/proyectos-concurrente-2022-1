// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>

#include "./Matrix.hpp"
#include "./Terrain.hpp"

class Row;

class Value;

class Map {
public:
    Map(size_t rows, size_t cols, Matrix<char> area)
            : rows(rows), cols(cols), area(std::move(area)) {
    }

    void step();

    size_t getTime();

    const size_t rows;
    const size_t cols;

private:
    size_t time{0};

    Matrix<char> area;

    void reviewCurrentValue(size_t currentRow, size_t currentCol, size_t startRow, size_t endRow, size_t startCol, size_t endCol);

    void returnNeighboursCounters(char value, char neighbour, size_t &tree, size_t &lake, size_t &madow);

    char returnValueFromRule(char value, size_t numberTrees, size_t numberLakes, size_t numberMeadows);
};

void Map::step() {

    for (size_t x = 0; x < rows; x++) {
        for (size_t y = 0; y < cols; y++) {
            size_t move = 1;
            size_t startRow = 0;
            size_t startCol = 0;
            size_t endRow = (rows - 1);
            size_t endCol = (cols - 1);

            if (x - move > 0) {
                startRow = x - move;
            }

            if (y - move > 0) {
                startCol = y - move;
            }

            if (x + move < rows) {
                endRow = x + move;
            }

            if (y + move < cols) {
                endCol = x + move;
            }

            reviewCurrentValue(x, y, startRow, endRow, startCol, endCol);
;        }
    }

    ++time;

    /* Idea para recorrer el mapa
     * for (int x = 0; x < 10; x++)
            {
                for (int y = 0; y < 10; y++)
                {
                    int movimiento = 1;
                    int puntoInicioFila = 0;
                    int puntoInicioColumna = 0;
                    int puntoFinalFila = 9;
                    int puntoFinalColumna = 9;

                    if (x - movimiento > 0)
                    {
                        puntoInicioFila = x - movimiento;
                    }

                    if (y - movimiento > 0)
                    {
                        puntoInicioColumna = x - movimiento;
                    }

                    if (x + movimiento < 10)
                    {
                        puntoFinalFila = x + movimiento;
                    }

                    if (y + movimiento < 10)
                    {
                        puntoFinalColumna = x + movimiento;
                    }

                    for (int i = puntoInicioFila; i < puntoFinalFila; i++)
                    {
                        for (int j = puntoInicioColumna; j < puntoFinalColumna; j++)
                        {
                            if (i == x && j == y)
                            {
                                continue;
                            }
                            Logica de negocio

                        }
                    }
                }
            }
     *
     * */


    // revisar area:
    // 	for size_t i [i][j]
    // // revisar cuadricula:
    // // // switch(terreno): si la celda es "terreno",
    // // // 	correr los isInundacion, isSecacion...
}

void Map::reviewCurrentValue(size_t currentRow, size_t currentCol, size_t startRow, size_t endRow, size_t startCol, size_t endCol) {
    size_t tree { 0 }, lake { 0 }, madow { 0 };

    for (int i = startRow; i < endRow; i++) {
        for (int j = startCol; j < endCol; j++) {
            if (i == currentRow && j == currentCol) {
                continue;
            }

            returnNeighboursCounters(area[currentRow][currentCol], area[i][j], tree, lake, madow);

        }
    }

    area[currentRow][currentCol] = returnValueFromRule(area[currentRow][currentCol], tree, lake, madow);
}

void Map::returnNeighboursCounters(char value, char neighbour, size_t &tree, size_t &lake, size_t &madow) {

    if ((value == Terrain::tree && neighbour == Terrain::lake) ||
            ((value == Terrain::tree && neighbour == Terrain::tree))) {
        if (neighbour == Terrain::lake) {
            lake = lake + 1;
        } else {
            tree = tree + 1;
        }
    }

    if (value == Terrain::lake && neighbour == Terrain::lake) {
        lake = lake + 1;
    }

    if (value == Terrain::madow && neighbour == Terrain::tree) {
        tree = tree + 1;
    }
}


char Map::returnValueFromRule(char value, size_t numberTrees, size_t numberLakes, size_t numberMeadows) {
    /*
 *  "a" con >3 vecinos "l" -> "l"
 *  "l" con <3 vecinos "l" -> "-"
 *	"-" con >2 vecinos "a" -> "a"
 *	"a" con >4 vecinos "a" -> "-"
 */

    char result { value };

    switch (value)
    {
        case Terrain::tree :
            if(numberLakes > 3){
                result = Terrain::lake;
            } else if (numberTrees > 4){
                result = Terrain::madow;
            }
            break;
        case Terrain::lake:
            result = numberLakes < 3 ? Terrain::madow : value;
            break;
        case Terrain::madow:
            result = numberTrees > 2 ? Terrain::tree : value;
            break;
        default:
            break;
    }

    return  result;
}


