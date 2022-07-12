// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>

#include "./Matrix.hpp"
#include "./Terrain.hpp"

class Row;
class Value;

class Map
{
public:
	Map(size_t rows, size_t cols, Matrix<Terrain> area)
			:rows(rows), cols(cols), area(std::move(area))
	{
	}

	void step();

	size_t getTime();

	const size_t rows;
	const size_t cols;
private:
	size_t time{ 0 };
	Matrix<Terrain> area;
};

void Map::step()
{

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

	++time;
}
