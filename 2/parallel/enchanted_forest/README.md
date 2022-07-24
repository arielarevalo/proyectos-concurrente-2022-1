# Enchanted Forest

Este proyecto consiste en un programa capaz de procesar un archivo job el cual
contiene una lista de archivos map, por procesar. Cada map consiste en una
matriz de caracteres que representan un cuadro de un mapa que detalla un
bosque encantado. El mapa del bosque se procesa de acuerdo a las siguientes
reglas:

Cada medianoche el bosque cambia, y al día siguiente los ciudadanos
encuentran un lugar diferente para recrearse. El terreno del bosque cambia
de acuerdo a uno de los siguientes casos:

* Inundación: Si la celda tiene un árbol y al menos 4 vecinos que son lago
  encantado, entonces el lago ahoga el árbol, y pasa a ser lago encantado.
* Sequía: Si la celda es lago encantado y tiene menos de 3 vecinos que sean
  lago encantado, entonces el lago se seca y pasa a ser pradera.
* Reforestación: Si la celda es pradera y tiene al menos 3 vecinos árboles,
  las semillas tendrán espacio para crecer y la celda se convierte en árbol.
* Hacinamiento: Si la celda es un árbol y tiene más de 4 vecinos árbol, el
  exceso de sombra evita que crezca y entonces pasa a ser pradera.
* Estabilidad: Cualquier otra situación, la celda permanece como está.

Para decidir cuantas noches procesar un mapa, cada línea del job contiene
tanto el nombre del archivo map por procesar, así como el número de noches.
Si el número es positivo, el programa debe producir un archivo con el
estado del mapa en cada amanecer. Cuando el número de noches es negativo,
el programa debe aplicar las reglas mágicas esa cantidad de noches, y producir
como salida un único archivo con el mapa resultante después de la última
medianoche.

## Instrucciones de uso

Primeramente, debemos saber como espera el programa su archivo de entrada.
Además de la descripción anterior, mostramos un ejemplo de un archivo job de
entrada seguidamente:

```
job001.txt

map001.txt 2
map002.txt -100
map003.txt -20000
```

La carpeta donde se encuentra el archivo job debe contener todos los mapas
por procesar para ese job. Mostramos, entonces, un ejemplo de un archivo map:

```
map001.txt

7 7
-------
-l--l--
-ll----
-l-----
---laa-
-aa-al-
a-a----
```

Con un archivo job de entrada y sus respectivos maps a la mano procedemos a
compilar el programa con el comando `mpic++`, y podemos correrlo ejecutando
`mpiexec -np [Número de procesos] ./enchanted_forest [Dirección de archivo] [Número de hilos]`. Seguidamente, se estará almacenando en el directorio donde se encuentra el archivo job un
directorio con el nombre del archivo job, el cual contiene los maps procesados solicitados dentro del archivo job. Como ejemplo de archivo de salida, se presenta el siguiente:

```
map001-1.txt

-------
-------
-ll----
-------
----aa-
-aaaa--
aaaa---
```

## [Diseño](./design/README.md)

## [Análisis de Rendimiento](./design/PARALELIZACION.md)

## Créditos

El programa fue desarrollado en su totalidad por Ariel Arévalo Alvarado
(<ariel.arevalo@ucr.ac.cr>) y Pablo Madrigal Ramírez (<pablo.
madrigalramirez@ucr.ac.cr>).
