[Reporte](../report/README.md)

# Tetris Solver

Este proyecto consiste en un programa capaz de resolver una jugada de
Tetris, con el problema enunciado de la siguiente forma:

> Dado un estado inicial de área de juego, profundidad por consultar, y
> formas de Tetris próximas por ubicar, hallar la permutación entre las
> posibles ubicaciones y orientaciones válidas para cada forma siguiente de
> tal forma que se maximice el puntaje obtenido.

O dicho de otra forma, el programa recibe un archivo con un estado de juego
inicial, lo cual consiste en un número de identificación, una cantidad de
próximas piezas por las cuales consultar, las dimensiones y la matriz que
representan el área de juego, y una serie de piezas siguientes las cuales
intentar agregar al área de juego. El programa prueba todas las maneras en
que estas piezas se pueden agregar al área de juego, en el orden en que se
escriben en el archivo, y devuelve una serie de archivos que detallan los
movimientos que dan la mayor cantidad de puntos.

## Instrucciones de uso

Primeramente, debemos saber como espera el programa su archivo de entrada.
Además de la descripción anterior, mostramos un ejemplo de un archivo de
entrada seguidamente:

```
tetris_state.txt

1650259796944
1
20
10
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0001111000
0000040000
0004440000
0000700000
0007770000
0002200000
0020220000
0220066000
0777742240
7
I
Z
I
J
S
L
S
```

Con un archivo de entrada a la mano procedemos a compilar el
programa con el comando `cmake`, y podemos correrlo ejecutando
`./tetris_player [Dirección de archivo] [Número de hilos]`. Seguidamente, se
estarán almacenando en el directorio `put` los archivos que representan las
movidas las cuales hacer para obtener la mayor cantidad de puntos, o se 
devolverá por consola un mensaje indicando que no se puede completar el 
juego solicitado a la profundidad solicitada. Como ejemplo de archivo de 
salida, se presenta el siguiente:

```
tetris_play_0.txt

1650259796944
I
1
20
10
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0001111000
0000040000
0004440000
0000700000
0007770000
1002200000
1020220000
1220066000
1777742240
```

## [Diseño](./design/README.md)

## Créditos

El programa fue desarrollado en su totalidad por Ariel Arévalo Alvarado
(<ariel.arevalo@ucr.ac.cr>)
