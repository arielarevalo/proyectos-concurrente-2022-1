
# Diseño Tetris Player

    Su solución debe tener un buen diseño concurrente expresado mediante diagramas de flujo de datos y UML.
    Su diseño debe ser concreto y mostrar el flujo de datos de entrada y salida, incluyendo nombres de los métodos más importantes. En el diagrama UML debe quedar claro cuáles fueron todas las clases utilizadas y sus atributos. Los diseños deben guardarse en la carpeta design/ dentro de su proyecto 1.
    Al leer el documento de diseño, una persona desarrolladora tendrá una comprensión de alto nivel de la solución. Es muy útil para la fase de implementación o para futuros miembros de un equipo con el fin de poder comprender el diseño de la solución.

## Requisitos

### Funcionales:

* Recibir un archivo con el siguiente formato como entrada:
    1. ID
    2. Profundidad (D)
    3. Filas (N)
    4. Columnas (M)
    5. Área de juego (N x M)
    6. Número de formas siguientes (K)
    7. Una fila por cada forma siguiente con su caracter

* Emitir un archivo por cada nivel de profundidad ("tetris_play_d.txt") con el
  siguiente formato como salida:
    1. ID (Mismo)
    2. Caracter de la forma recién ubicada
    3. Número de rotación de la forma recién ubicada
    4. Filas
    5. Columnas
    6. Área de juego actualizada

* Retornar solamente las ubicaciones hasta llegar a algún nivel donde no haya
  solución
  válida. (Los/cuántos movimientos por imprimir se sabrán antes de iniciar la
  generación de archivos)

* Utilizar búsqueda en profundidad por D. En cada nivel:
    * Hallar puntajes para cada combinación de ubicación y orientación de la
      dada forma.
    * Repetir para cada posible subnivel, con su respectiva forma, a partir de
      cada una de las anteriores combinaciones.
    * Al terminar con un nivel, comparar cada combinación resultante, y retornar
      la de puntaje máximo.

* Utilizar una heurística existente de las indicadas, y dar crédito, para
  determinar un "puntaje máximo".

### Arquitecturales:

* Validar entradas.
* Utilizar estructuras de datos a la medida o adaptadas, con crédito.
* No optimizar para tiempo de ejecución.
* Hacer pruebas, así como análisis con las herramientas vistas en clase.
* Diseñar de forma desacoplada; preparar con UML.
* Utilizar mapeo de bloque o mapeo dinámico para asignar a los hilos sus tareas.

## Implementación

Se parte de una división singular para la lógica del programa, entre el
filer y el scorer. Además, se utilizan las siguientes estructuras de datos:

    GameState, PlayState, Matrix, Tetrimino

El propósito de cada una de estas partes respecto a los requisitos es el
siguiente:

| Clase     | Propósito                                                                                                                                                 |
|-----------|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
| Filer     | Validación de entradas. Conversión de archivo a `InState` y de `PlayState` a archivo.                                                                     |
| Scorer    | Navega las permutaciones de los `PlayState` y puntúa cada `PlayState.playArea` a partir de [la heurística de Takado8](https://github.com/takado8/Tetris). |
| GameState | Contiene la información inicial del estado de juego.                                                                                                      |
| PlayState | Contiene la información de estado de una jugada.                                                                                                          |
| Tetrimino | Contiene la forma de un tetrimino como una matriz de caracteres.                                                                                          |
| Matrix    | Contiene la forma de una area de juego como una matriz de caracteres.                                                                                     |

### Filer

El algoritmo de `filer` consiste en la lectura y escritura de los contenidos de
un archivo por líneas. En el caso de lectura, las líneas son respectivamente
convertidas en las variables de un `GameState` para la construcción del
mismo. Esto se lleva a cabo a través de un número de ejecuciones de `cin`
basado en la especificación del archivo de entrada y el número de filas del
área de juego ASCII contenida en el mismo. Además, para el caso de escritura, se
lleva a cabo la necesaria funcionalidad de conversión de `PlayState` a
archivo a través de un proceso idéntico, pero inverso, al de lectura.

### Play State Factory

El algoritmo `PlayState.place( playState, tetrimino, rotation, column )`
explora la topografía que trazan la celda no-vacía más alta de cada columna
junto con las celdas no-vacías más altas de *w-1* columnas a la derecha de la
original, donde *w* es el ancho del `tetrimino` específico que se esté
intentando ubicar de forma válida en el área de juego `matrix` del `playState`
que recibe. Se asume, según los lineamientos de la tarea, que sólo es necesario
que esta "silueta", así como los bordes del área de juego, no obstruyan el
cuerpo del `tetrimino` si este se ubicase en el punto inicial de anclaje a la
izquierda. El `place`, entonces, intenta agregar el `tetrimino` de forma válida,
y si lo logra, retorna un 1, de lo contrario, 0.

### Scorer

#### Find Best Moves

El algoritmo `findBestMoves` se ocupa de navegar las permutaciones de cada
estado de juego a partir del estado de juego inicial. En cada nivel de
profundidad, empezando desde el -1, se hallan todos los posibles estados de
juego hijos/permutación del estado de juego de entrada con el método
`findBestChild`, y al llegar al nivel de profundidad indicado en el
archivo de entrada como el máximo se puntúa el estado de juego, y se
compara con el puntaje del estado de juego de puntaje máximo, guardado en
el arreglo `history` usando el método `compareToBest`. A la hora de ir
subiendo de vuelta los niveles de profundidad, el valor de retorno de
`isChildHighScore` indica si en el siguiente nivel más profundo al actual se
halló un estado de juego de puntaje máximo, o `HI_SCORE`, en cuyo caso se
agrega el estado de nivel actual al arreglo `history`.

    findBestMoves(GameState initial, PlayState current,
                    vector<PlayState> history, int currentDepth)
    finalDepth = initial->depth;
    if (currentDepth < finalDepth)
        findBestChild(initial, currentDepth, current, history)
        if (currentDepth > -1 && current.isChildHighScore())
            history[currentDepth] = new PlayState(current);
    else if (current_depth == final_depth)
        compareToBest(final_depth, current, history);

También cabe destacar que es precisamente este método el que los hilos son
capaces de ejecutar de forma individual, esto a partir del nivel cero de
profundidad de búsqueda.

### Diagrama

![Diagrama a nivel de main](UMLClass.PNG)

![Flow Chart a nivel de main](Flowchart.PNG)
