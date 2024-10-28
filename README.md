# MPI - Programming Assignments

Este repositorio contiene una serie de programas y ejemplos en C++ utilizando la biblioteca MPI (Message Passing Interface) para tareas de computación paralela. Los ejercicios cubren temas como el cálculo de histogramas, estimación de \(\pi\) mediante el método de Monte Carlo, suma global en estructura de árbol, y multiplicación matriz-vector con distribución en bloques de columnas.

## Contenido

### 1. Implementación de un Histograma en MPI
- **Descripción**: Implementa un programa en MPI donde el proceso 0 lee los datos de entrada y los distribuye entre los procesos. Cada proceso calcula su propio histograma parcial, y el proceso 0 reúne y suma los histogramas parciales para formar el histograma final.
- **Archivo**: `histogramaMPI.cpp`

### 2. Estimación de \(\pi\) mediante el Método de Monte Carlo en MPI
- **Descripción**: Estima el valor de \(\pi\) lanzando dardos aleatorios en un tablero cuadrado con un círculo inscrito. Cada proceso realiza una parte de los lanzamientos y cuenta cuántos caen dentro del círculo. El proceso 0 reúne los resultados para calcular la estimación final.
- **Archivo**: `montecarloMPI.cpp`

### 3. Implementación de Suma Global en Estructura de Árbol en MPI
- **Descripción**: Realiza una suma global en estructura de árbol. Incluye dos implementaciones: una para cuando el número de procesos es una potencia de dos, y otra para manejar cualquier número de procesos.
- **Archivos**: `potenciaDe2MPI.cpp` y `sumaGeneral.cpp`

### 4. Implementación de Multiplicación Matriz-Vector en MPI
- **Descripción**: Realiza la multiplicación de una matriz cuadrada por un vector utilizando una distribución en bloques de columnas. El proceso 0 distribuye la matriz entre los procesos, y cada proceso calcula su producto parcial, que luego es sumado para obtener el resultado final.
- **Archivo**: `vectorMatrizMPI.cpp`

## Requisitos
- **MPI**: Este repositorio requiere tener instalada la biblioteca MPI para ejecutar los programas.
- **Compilador C++**: Necesitas un compilador que soporte C++ y MPI (como `mpic++`).

## Compilación y Ejecución

Para compilar cualquiera de los archivos, utiliza el siguiente comando:
```bash
mpic++ -o nombre_programa archivo.cpp
