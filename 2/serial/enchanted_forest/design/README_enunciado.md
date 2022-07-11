# Proyecto02: El bosque encantado

El proyecto 2, sobre paralelismo de datos, concurrencia declarativa y distribución, se realiza en grupos de máximo tres personas y se debe entregar a más tardar el Sábado 23 de Julio a las 23:55. Las entregas tardías se tratarán como se estipula en la carta al estudiante.

Deben entregar su solución en el repositorio privado que para ese efecto ya creó cada grupo, dentro de una carpeta exclusiva para este proyecto, dado que el repositorio albergará los dos proyectos del curso. Deben asegurarse de que el profesor a cargo tenga acceso a dicho repositorio. Todos los miembros del grupo deben demostrar un nivel similar de participación en el desarrollo de la solución. Para esto se les recomienda hacer commits frecuentes con comentarios breves pero descriptivos de lo que representa cada commmit.

## Descripción del problema

Un rey generoso de una isla muy lejana, estaba preocupado porque sus ciudadanos estaban aburridos de la monotonía de la pequeña isla. Tuvo la idea de encantar el bosque que está alrededor de su castillo, de tal forma que cada medianoche el bosque cambie y al día siguiente los ciudadanos encuentren un lugar diferente para recrearse. A su mago le pareció genial la idea, pero no sabe qué reglas incluir en el hechizo para que el bosque se mantenga en equilibrio y no llegue a convertirse en un desierto o una selva impenetrable. Si tuvieran alguna forma de ver el efecto de las reglas a futuro, podrían decidir el hechizo con mayor seguridad.

El rey tiene un mapa del bosque como el extracto que puede verse más adelante en el ejemplo de entrada (*map001.txt*). El mapa ilustra lo que hay en cada metro cuadrado de la isla, a los que les llaman celdas. Una celda puede contener un árbol mágico ('a'), un trozo de lago encantado ('l'), o pradera donde pueden transitar los ciudadanos ('-'). Las dimensiones del mapa se encuentran en la primera línea de entrada, indicadas como el número de filas y columnas.

Las reglas que quieren probarse en cada medianoche trabajan en una celda a la vez y consideran las 8 celdas vecinas que tiene alrededor. Las reglas son las siguientes.

* **Inundación:** Si la celda tiene un árbol y al menos 4 vecinos que son lago encantado, entonces el lago ahoga el árbol, y pasa a ser lago encantado.
* **Sequía:** Si la celda es lago encantado y tiene menos de 3 vecinos que sean lago encantado, entonces el lago se seca y pasa a ser pradera.
* **Reforestación:** Si la celda es pradera y tiene al menos 3 vecinos árboles, las semillas tendrán espacio para crecer y la celda se convierte en árbol. 
* **Hacinamiento:** Si la celda es un árbol y tiene más de 4 vecinos árbol, el exceso de sombra evita que crezca y entonces pasa a ser pradera.
* **Estabilidad:** Cualquier otra situación, la celda permanece como está.

Se quiere que el programa ayude a probar las reglas mágicas anteriores en varios mapas. El programa recibe una orden de trabajo que el mago quiere probar. La orden de trabajo es un archivo cuyo nombre se envía por argumento de línea de comandos. En su contenido, la orden de trabajo lista algunos mapas iniciales de la isla, seguida de un número que indica la cantidad de medianoches en las que quiere probarse el efecto de las reglas del potencial hechizo.

Ejemplo de una orden de trabajo *job001.txt*:

```
map001.txt 2
map002.txt -100
map003.txt -20000
```

En la orden de trabajo *job001.txt* el número que sucede al nombre de archivo indica la cantidad de noches en las que se debe probar el efecto de las reglas. Si el número es positivo, el programa debe producir un archivo con el estado del mapa en cada amanecer. Por ejemplo, para el primer mapa *map001.txt*, se solicita probar el efecto en 2 noches. En tal caso, el programa produce un archivo de salida con el mapa resultante a la primera medianoche con nombre *map001-1.txt*, y el resultado de la segunda medianoche en el archivo *map001-2.txt*. Estos números positivos permiten al mago rastrear el efecto de las reglas paso a paso.

Cuando el número de noches es negativo, como -100 para el *map002.txt*, el programa debe aplicar las reglas mágicas esa cantidad de noches, y producir como salida un único archivo con el mapa resultante después de la última medianoche, que para el segundo mapa sería *map002-100.txt*. Estos números negativos permiten al mago conocer si a largo plazo sus reglas producen una topografía razonable o si debe cambiarlas.

Note que el nombre del archivo de salida usa como prefijo el nombre del archivo mapa, seguido por un guión y el número de medianoche. Por ejemplo el archivo *map003.txt* representa el mapa de la isla en su estado actual, lo que equivale a la medianoche cero (*map003-0.txt*). El archivo *map003-20000.txt* representa esa misma isla 20,000 medianoches después (casi 55 años).

En cuanto a los archivos de mapa, como *map001.txt*, contienen la isla o un trozo de ella representado como una matriz de caracteres. En el primer renglón del archivo se indica la cantidad de filas y columnas de la matriz. Cada carácter en los renglones subsecuentes corresponden a una celda de la matriz (a excepción de los cambios de línea). Por ejemplo:

Contenido del archivo *map001.txt*:

```
7 7
-------
-l--l--
-ll---- 
-l----- 
---laa- 
-aa-al-
a-a----
```

Ejemplo de salida *map001-1.txt*:

```
-------
-------
-ll----
-------
----aa-
-aaaa--
aaaa---
```

## Solución orientada a objetos [30%]

Se recomienda primero resolver el problema con una solución serial enfocada en corrección y utilizando el paradigma de programación orientada a objetos en C++. Debe realizarse el diseño acorde en  UML  con diagramas de clases y de interacción entre objetos. Los métodos algorítmicos que implican poca interacción entre objetos pueden diseñarse con pseudocódigo. Algunas ideas a considerar en su diseño:

* Construir un bosque vacío con dimensiones de filas y columnas arbitrarias dadas por parámetro.
* Saber la cantidad de filas y columnas que hay en el bosque.
* Actualizar una celda del bosque en una medianoche. Considere un método privado que recibe por parámetros la fila y la columna que se quiere actualizar. El método actualiza la celda en medianoche de acuerdo a las reglas.
Actualizar todo el bosque en una medianoche.
* Si el bosque es muy grande, permitir a dos o más trabajadores actualizar regiones distintas del bosque sin afectarse entre ellos, o trabajar en bosques distintos.

Los diseños deben guardarse en la subcarpeta design/ dentro de la carpeta para el proyecto02 en su repositorio de control de versiones. Exporte sus diseños a imágenes e incrústelas en un archivo design/readme.md. En este documento escriban las anotaciones o explicaciones que ayuden a comprender la solución (los diagramas) y algoritmos (pseudocódigos).

Implementen la versión serial en C++ aplicando las buenas prácticas de programación y documentación. La aplicación valida las entradas, como números mal formados, fuera de rango, matrices incompletas o inválidas, son reportadas con mensajes de error, en lugar de caerse o producir resultados incorrectos. Asegúrense de que la aplicación pase los casos de prueba, y no genere diagnósticos del linter, sanitizers o memcheck, como se indica en la sección de "Aspectos transversales".

## Diseño de la aplicación distribuida-híbrida [15%]

Una vez construida su solución serial, realicen un diseño concurrente que intente incrementar el desempeño usando las tecnologías OpenMP y MPI.

Cree un documento en la carpeta design/ del Proyecto02 con un archivo en formato markdown con una descripción de no más de 500 palabras que incluya al menos los siguientes puntos:

* Descripción del tipo descomposición y mapeo a utilizar para la parte distribuida del programa (mediante MPI).
* Descripción del tipo descomposición y mapeo a utilizar para la parte concurrente del programa (mediante OpenMP).
* Si utilizan separación de asuntos, explicar qué hace cada tipo de proceso y en qué nodo se ejecuta.
* Describa cómo los procesos van a manejar la entrada y la salida.

## Implementación del programa distribuido [30%]

Implemente la paralelización con MPI y con OpenMP tal y como lo planteó en su diseño. El programa debe funcionar para un número N de procesos/nodos. La cantidad de CPUs a utilizar debe poder ser controlada por argumento de línea de comandos, en caso de omitirse se debe suponer la cantidad de CPU disponibles en el equipo donde corre el programa.

Asegúrese de que la solución pasa las pruebas de corrección. La solución debe pasar los casos de prueba, y no tener malas prácticas como espera activa, condiciones de carrera, bloqueos mutuos, inanición, serialización innecesaria, o ineficiencia. Se recomienda usar herramientas como tsan para validar el correcto funcionamiento del programa y  cpplint para validad el apego a una convención de estilo del código fuente.  

## Mediciones de Rendimiento [15%]

Se sugiere seguir los siguientes pasos durante la realización del proyecto para facilitar las mediciones de rendimiento:

### Paso 1.
Medir el rendimiento de la versión serial con un caso de prueba grande (por ej. el caso de prueba *job002.txt*), siguiendo el procedimiento indicado en la [Tarea03](https://github.com/jocan3/CI0117-2022-S1/tree/main/enunciados/tareas/03) con al menos tres corridas. Anotar los resultados en una hoja de cálculo. **Nota:** si no se implementó una versión serial puede usar la versión paralelizada con un único hilo y un único proceso.

### Paso 2. 
Diseñar la paralelización con MPI y OpenMP. Discutan y reflejen la optimización pretetendida en los diseños realizados en la fase serial (pseudocódigo o diagramas). Implementar la paralelización con MPI y OpenMP en el código fuente.

### Paso 3.
El programa distribuido debe pasar las pruebas de corrección. La solución debe pasar los casos de prueba, y no tener malas prácticas como espera activa, condiciones de carrera, bloqueos mutuos, inanición, serialización innecesaria, o ineficiencia.  

### Paso 4.
Medir el rendimiento de la versión paralelizada con los mismos métodos que el paso 1. Calcular el incremento de velocidad (*speedup*) y eficiencia. Cree un gráfico que incluya en el eje-x las dos soluciones (serial, distribuida), en el eje-y primario el incremento de velocidad, y en el eje-y secundario la eficiencia. Para la versión distribuida utilice al menos 4 nodos del Cluster y tantos hilos como núcleos hayan disponibles en la máquina (si el rendimiento empeora, puede probar con cantidades más bajas de hilos por ej. 8 ó 10). Incruste su gráfico en una sección "Análisis de rendimiento" de su README.md con su correspondiente discusión corta (500 palabras máximo).

Para las pruebas del programa distribuido utilice el clúster Kabré (Solicite al profesor una cuenta de acceso y refiérase al [manual de usuario del clúster](https://kabre.cenat.ac.cr/guia-usuario/) para más información, además de las explicaciones dadas en clase sobre cómo usarlo). Se adjunta un ejemplo del archivo .slurm para enviar solicitudes de trabajo en el clúster utilizando MPI.

Tanto para MPI como para OpenMP puede utilizar cualquiera de los métodos de mapeo disponibles en la tecnología de paralelización (bloque, cíclico, bloque cíclico, dinámico, y guiado). Se recomienda probar diferentes tipos y utilizar el que muestre mejor rendimiento en su solución.

## Estructura del proyecto [10%]

Buen uso del repositorio de control de versiones. Debe tener commits de cada integrante del equipo. Debe haber una adecuada distribución de la carga de trabajo, y no desequilibrios como que “un miembro del equipo sólo documentó el código”. Cada commit debe tener un cambio con un significado y un mensaje significativo.

Las clases, métodos, y tipos de datos deben estar documentados como se ha solicitado en las demás entregas de este curso. Documentar el código no trivial en los cuerpos de las subrutinas. Debe mantenerse la estructura de archivos y directorios característica de un proyecto de Unix:

- *bin/* folder con los ejecutables.
- *design/* folder con el diseño de la solución (Por ej. UML, pseudocódigo u otro).
- *src/* folder con el Código fuente (.h y .c).
- *test/* folder con archivos de casos de prueba (**Importante:** sólamente incluya el primer set de archivos, no subir archivos grandes)
- *Makefile* que compila y prueba con las diferente herramientas vistas en clase.
- *README.md* que describe el problema resuelto y el manual de usuario
- *.gitignore* debe ignorar los recursos que NO deben estar en control de versiones (por ej. archivos ejecutables, código binario, carpeta *build*, archivos del IDE, etc.)

Debe crear un archivo README.md en la raíz de la solución que indique el propósito de la misma. Agregar una sección "Compilar" al README.md que explique cómo compilar y correr su código. Agregar en una sección "Manual de usuario" cómo ejecutar la aplicación, describir los parámetros, y describir la salida esperada. Conviene agregar salidas textuales o capturas de pantalla. Incluir una sección de diseño en el README.md con el modelo estático (diagrama de clases) y, a modo opcional, el modelo dinámico (diagrama de secuencia o actividad) de su solución.

## Aspectos transversales

El peso en la evaluación de cada componente del avance se encuentra en los títulos anteriores. En cada uno de ellos se revisa de forma transversal:

- La corrección de la solución. Por ejemplo: comparar la salida del programa contra los casos de prueba.

- La completitud de la solución. Es decir, si realiza todo lo solicitado.

- La calidad de la solución ejecutable. Por ejemplo, no generar condiciones de carrera, espera activa, bloqueos mutuos, inanición (puede usar herramientas como helgrind, tsan). Tampoco accesos inválidos ni fugas de memoria (memcheck, asan, msan, ubsan). En caso de que estas herramientas muestren falsos positivos, documéntelos en el archivo README.md justificando el por qué no representan un problema.

- La calidad del código fuente, al aplicar las buenas prácticas de programación. Por ejemplo, modularizar (dividir) subrutinas o clases largas, y modularizar los archivos fuente (varios .hpp y .cpp). Apego a una convención de estilos (cpplint). Uso identificadores significativos. Inicialización de todas las variables. Reutilización de código.