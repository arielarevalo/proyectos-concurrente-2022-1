# Proyecto01: [tetris_player]. Entrega #1

El Proyecto01 será sobre concurrencia de tareas y paralelismo de datos, sin embargo, esta primera entrega consistirá en una versión **serial** que será optimizada mediante concurrencia en la siguiente entrega. Se realizará en grupos de máximo tres personas y se utilizará el lenguage de programación C++. Entregar a más tardar el 11 de Junio a las 23:59. Las entregas tardías se tratarán como se estipula en la carta al estudiante.

## Repositorio y archivos
Deben entregar su solución en el repositorio privado de Github que para ese efecto ya creó cada grupo, dentro de una carpeta exclusiva para este proyecto, dado que el repositorio albergará los dos proyectos del curso. Deben asegurarse de que el docente tenga acceso a dicho repositorio. Todos los miembros del grupo deben demostrar un nivel similar de participación en el desarrollo de la solución. Cada participante debe asegurarse de hacer commits frecuentes, que impliquen un aporte, con mensajes breves pero descriptivos. Debe haber una adecuada distribución de la carga de trabajo, y no desequilibrios como que "un miembro del equipo sólo documentó el código". Pueden hacer uso de *branches* y cualquier otra práctica de control de versiones que permita Github.

Los archivos del programa deben seguir la estructura de archivos y directorios característica de un proyecto de Unix, descrita en el enunciado de la tarea01, y debe ser mantenida por los miembros del equipo. Utilice el Makefile incluido en este enunciado para lograr la estructura deseada y compilar código en C++.

Las clases, métodos, y tipos de datos, deben estar documentados al igual que el código no trivial en los cuerpos de las subrutinas. Su solución debe ser modular, dividiendo el código en subrutinas, clases, y archivos. El código debe tener un estilo consistente y no generar diagnósticos del linter (cpplint).

## Descripción del problema

En la tarea01 se creó la lógica para que, dado un archivo de entrada representando un estado de juego de Tetris, se determinara la posición óptima para colocar las siguientes figuras tomando en cuenta parámetros como la profundidad y haciendo una búsqueda en profundidad mediante fuerza bruta (ver detalles del juego en la descripción de la tarea01).

El objetivo del Proyecto01 es crear un jugador del juego Tetris que utilice la misma lógica implementada en la tarea01, pero que sea capaz de procesar múltiples archivos de entrada, uno por uno, y generando los archivos de salida en tiempo real. Tanto los archivos de entrada como los archivos de salida deben tener exactamente el mismo formato que los de la tarea01.

Para ello, se debe realizar lo siguiente:

### 1. Migración de la lógica del tetris-solver-serial

Migrar la lógica implementada en la tarea01 a código en C++. Dado que la tarea se realizó de forma individual, los miembros del equipo deciden cuál de las soluciones utilizarán en el proyecto. Tenga en cuenta que, aunque mucha de la lógica y notación es similar para C y C++, se deben realizar los cambios necesarios para que el programa sea Orientado a Objetos, incluyendo la lógica de las figuras dada por el profesor. De igual manera, puede hacer uso de cualquier biblioteca y estructuras de datos disponibles en C++ que faciliten la implementación.

### 2. Implementación de un File watcher

Implementar un *file watcher* (observador de archivos) para leer los archivos de entrada. Puede utilizar cualquier biblioteca disponible en C++ y en el sistema operativo Linux para implementarlo (por ej. *inotify*). Este *file watcher* deberá observar continuamente un folder específico y cada vez que un archivo de entrada con el nombre "tetris_state.txt" aparezca en dicho folder (colocado ahí por un cliente, usuario u otro proceso externo), se deberá realizar lo siguiente:

- Leer el archivo
- Procesar (*parse*) el contenido del archivo, el cual debe estar en el mismo formato de entrada explicado en la tarea01 y, una vez extraidos los valores, borrar el archivo (esto para permitir que aparezca otro archivo de entrada con el mismo nombre)
- Iniciar el algoritmo para determinar la jugada óptima tal como se implementó en la tarea01.
- Generar el o los archivos de salida correspondientes (siguiendo mismo formato de la tarea01). Los archivos de salida se deben generar en el mismo folder de entrada y no se deben preocupar si ya existe uno con el mismo nombre, simplemente sobreescriben los ya existentes.

Luego de que se realiza todo lo anterior, el *filewatcher* regresa a monitorear el mismo folder y en caso de que aparezca otro archivo con el nombre "tetris_state.txt" vuelve a iniciar todo el proceso para generar las salidas. El programa solo procesa un archivo a la vez y no se debe leer otro archivo mientras haya uno actualmente siendo procesado (si llegara un archivo de entrada mientras se está buscando una solución, simplemente se ignora, no se deben "encolar").

## Análisis

Los miembros del equipo elaboran el documento de análisis en un README.md, en el cual debe quedar claro a un lector ajeno al proyecto, el problema que se resuelve, y quienes son los integrantes del equipo. Proveer además un manual de uso, que incluye cómo compilar el programa y cómo correrlo (por ejemplo, explicar los argumentos en línea de comandos que recibe) y cómo detenerlo (tanto con Ctrl+C como el comando kill).

El documento puede incluir también capturas de pantallas o videos de la ejecución, ya sea en la terminal o utilizando la interfaz gráfica dada por el profesor (ver sección de pruebas).

## Diseño

Su solución debe tener un buen diseño concurrente expresado mediante diagramas de flujo de datos y UML.

Su diseño debe ser concreto y mostrar el flujo de datos de entrada y salida, incluyendo nombres de los métodos más importantes. En el diagrama UML debe quedar claro cuáles fueron todas las clases utilizadas y sus atributos. Los diseños deben guardarse en la carpeta design/ dentro de su proyecto 1.

Al leer el documento de diseño, una persona desarrolladora tendrá una comprensión de alto nivel de la solución. Es muy útil para la fase de implementación o para futuros miembros de un equipo con el fin de poder comprender el diseño de la solución.

En el documento de análisis para el proyecto, agregue una sección de diseño y un enlace el documento de diseño. Esto permitirá la navegabilidad desde la raíz del repositorio al proyecto 01, y de éste hacia su diseño.

## Implementación Orientada a Objetos

En este primer avance no se requiere que la solución sea concurrente, se debe utilizar una versión serial (tarea01). Sin embargo, la solución debe estar implementada en el paradigma de programación orientada a objetos. Se recomienda una separación en clases de C++ que representen el tablero del juego, las figuras, el solucionador, manejadores de archivos, etc.

Debe asegurarse que la funcionalidad sea la misma que la de la versión serial de la tarea01 utilizada para este proyecto (Dado un archivo de salida debe generar los mismos archivos de salida que se generarían en la tarea01).

Debe seguir buenas prácticas de programación. Haga uso de los conceptos de Programación Orientada a Objetos y herramientas de C++ necesarios para resolver su solución (Por ej. herencia, creación de interfaces, tipos definidos, sobrecarga de operadores). Cree archivos de encabezado para exponer los métodos y atributos públicos (archivos .hpp) y la implementación de los mismos en archivos de código (.cpp).

## Pruebas
Utilice los mismos casos de prueba creados para las tareas. La diferencia es que debe colocar los archivos en el folder corespondiente y esperar a que el *file watcher* lo "consuma" y lo procese. Asegúrese de crear copias de estos archivos de entrada dado que su programa los borrará luego de procesarlos.

### Pruebas utilizando interfaz gráfica

Para este proyecto, el profesor adjunta en este enunciado una versión beta de una aplicación web gráfica del juego de Tetris implementada para propósito de pruebas. Dicha aplicación permite la generación aleatoria de un juego en un tablero de 20x10 con 7 piezas y genera archivos de estados en el folder establecido y utilizando la profundidad deseada por el usuario. La solución consta de una aplicación web (tetris-webapp) y un web service (tetris-api). Para levantar la aplicación se deben seguir los siguientes pasos:

1. Descargue el archivo tetris-api-v1.x.zip en su máquina Linux y extraiga el contenido en un folder con el nombre *tetris-api*.
2. Instale [NodeJS](https://nodejs.org/en/download/) y NPM en su máquina Linux. Puede hacerlo desde la terminal buscando los comandos especificos para su distribución. Por ejemplo, para distribuciones Debian puede seguir [estos pasos](https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-debian-10).
3. Ingrese al folder *tetris-api* desde la terminal y ejecute los siguientes comandos:

```
npm install
```
Para instalar las dependencias del web service. Una vez que finalice, ejecute el servicio corriendo:

```
node App.js
```

Debe aparecer un mensaje indicando que el web service está corriendo en el puerto 3001.

4. Descarge el archivo comprimido tetris-webapp-vx.x.zip en su máquina Linux y extraiga el contenido en un folder con el mismo nombre. Abra el archivo *index.html* para iniciar la aplicación web.

5. Para probar su jugador de Tetris, asegúrese de que su programa del Proyecto01 esté corriendo.

6. En la aplicación web ingrese la ruta completa del folder en el cual su programa está esperando archivos de entrada. Selecione el número de jugadas (*# of plays*) que representan la profundidad de búsqueda deseada. Haga click en el botón "Start" para iniciar el juego.
7. La aplicación web empezará a generar archivos de estados de Tetris en el folder establecido y leerá las salidas de su programa para realizar la jugada siguiente (sólamente toma en cuenta la jugada del archivo "tetris_play0.txt" para realizar una jugada a la vez).

**Nota:** Tome en cuenta que esta interfaz gráfica es una guía para ayudar con las pruebas de su programa y en la mayoría de los casos debería mostrarles correctamente cómo su jugador de tetris realiza jugadas hasta terminar la partida (*Game Over*). Sin embargo, podrían haber situaciones en las que la interfaz no funcione correctamente dado que es una versión beta. Puede consultar con el profesor en casos en los que se encuentre algún defecto.


## Evaluación
La revisión de avance será mediante una sesión interactiva por videoconferencia. Los miembros del equipo primero realizan una presentación del programa y corren las pruebas de validez. Luego se revisan algunas regiones de código elegidas por el docente. Para esta etapa se debe mostrar en pantalla tanto el diseño como el código al mismo tiempo. Todos los miembros del equipo deben participar durante la sesión. Se dará crédito a la evidencia presentada en cada aspecto de este enunciado y se distribuye en los siguientes rubros.

[5%] Buen uso y aporte equilibrado del repositorio (commits, ignores, tags) y directorios.

[10%] Análisis: README.md, problema, integrantes, manual de usuario, capturas.

[15%] Diseño: documento de diseño, flujo de datos, clases en UML, pseudocódigo.

[35%] Implementación del programa Orientado a Objetos y funcionalidad.

[15%] Buen uso de la memoria (Sanitizers y Valgrind).

[10%] Casos de prueba (Incluyendo demostración utilizando la interfaz gráfica).

[5%] Modularización en subrutinas y archivos (.hpp, .cpp). Apego a una convención de estilos (cpplint).

[5%] Documentación de interfaces e implementaciones en el código (algoritmos).

Recuerde que en todos los rubros se evalúan las buenas prácticas de programación.
