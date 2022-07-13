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
    Map(size_t rows, size_t cols, Matrix<Terrain> area)
            : rows(rows), cols(cols), area(std::move(area)) {
    }

    void step();

    size_t getTime();

    const size_t rows;
    const size_t cols;
private:
    size_t time{0};
    Matrix<Terrain> area;

    Terrain returnValueFromRule(Terrain value, size_t numberTrees, size_t numberLakes, size_t numberMeadows);
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

            size_t arbol = 0, lago = 0, pradera = 0;

            for (int i = startRow; i < endRow; i++) {
                for (int j = startCol; j < endCol; j++) {
                    if (i == x && j == y) {
                        continue;
                    }
                    // Cargar contadores = business logic

                    char value = area[i][j];
                }
            }
        }
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

    /*
     *  "a" con >3 vecinos "l" -> "l"
     *  "l" con <3 vecinos "l" -> "-"
     *	"-" con >2 vecinos "a" -> "a"
     *	"a" con >4 vecinos "a" -> "-"
     */
}

Terrain Map::returnValueFromRule(Terrain value, size_t numberTrees, size_t numberLakes, size_t numberMeadows) {
    switch(value) //donde opción es la variable a comparar
    {
        case Terrain::arbol: //Bloque de instrucciones 1;
            break;
        case valor2: //Bloque de instrucciones 2;
            break;
        case valor3: //Bloque de instrucciones 3;
            break;
            //Nótese que valor 1 2 y 3 son los valores que puede tomar la opción
            //la instrucción break es necesaria, para no ejecutar todos los casos.
        default: //Bloque de instrucciones por defecto;
            //default, es el bloque que se ejecuta en caso de que no se de ningún caso
    }
}


