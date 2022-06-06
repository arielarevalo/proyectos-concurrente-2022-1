# Tetris Solver

    Los miembros del equipo elaboran el documento de análisis en un README.md, en el cual debe quedar claro a un lector ajeno al proyecto, el problema que se resuelve, y quienes son los integrantes del equipo. Proveer además un manual de uso, que incluye cómo compilar el programa y cómo correrlo (por ejemplo, explicar los argumentos en línea de comandos que recibe) y cómo detenerlo (tanto con Ctrl+C como el comando kill).
    El documento puede incluir también capturas de pantallas o videos de la ejecución, ya sea en la terminal o utilizando la interfaz gráfica dada por el profesor (ver sección de pruebas).
    En el documento de análisis para el proyecto, agregue una sección de diseño y un enlace el documento de diseño. Esto permitirá la navegabilidad desde la raíz del repositorio al proyecto 01, y de éste hacia su diseño.

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
intentar agregar al área de juego. El programa prueba todas las formas en
que estas piezas se pueden agregar al área de juego, en el orden en que se
escriben en el archivo, y devuelve una serie de archivos que detallan los
movimientos que dan la mayor cantidad de puntos.

## Instrucciones de uso

Primeramente, debemos saber como espera el programa su archivo de entrada.
Además de la descripción anterior, mostramos un ejemplo de un archivo de
entrada seguidamente:

```
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

Con un archivo de entrada `archivo.txt` procedemos a compilar el programa con el
comando `make`, y podemos correrlo para nuestro archivo ejecutando
`./bin/tetris_solver_serial [Path]/archivo.txt`. Cabe destacar que el programa
solamente se puede correr con un archivo. Seguidamente, se estarán
almacenando en el directorio `output` los archivos que representan las
movidas las cuales hacer para obtener la mayor cantidad de puntos. Como
ejemplo de archivo de salida, se presenta el siguiente:

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

## Créditos

El programa fue desarrollado en su totalidad por Ariel Arévalo Alvarado
<ariel.arevalo@ucr.ac.cr>, con la heurística utilizada para puntuar los
estados de juego de Tetris tomada de
[un proyecto de Takado8](https://github.com/takado8/Tetris), y un logger
simple [tomado de Stack overflow](https://stackoverflow.com/questions/6508461/logging-library-for-c)
a nombre de Edwin Buck.

