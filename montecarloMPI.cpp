#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, comm_sz;
    long long int number_of_tosses, local_tosses, number_in_circle = 0, local_in_circle = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Proceso 0 lee el numero total de lanzamientos
    if (rank == 0) {
        cout << "Ingrese el numero total de lanzamientos: ";
        cin >> number_of_tosses;
    }

    // Broadcasting del numero total de lanzamientos a todos los procesos
    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Calcular lanzamientos locales
    local_tosses = number_of_tosses / comm_sz;
    
    // Semilla de numeros aleatorios
    srand(time(NULL) + rank);

    // Lanzar dardos aleatorios
    for (long long int toss = 0; toss < local_tosses; toss++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;  
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0; 
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1.0) {
            local_in_circle++;  // Incremento si el dardo cae dentro del circulo
        }
    }

    // Reduccion para sumar los dardos en el circulo en todos los procesos
    MPI_Reduce(&local_in_circle, &number_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0 calcula la estimacion de Pi
    if (rank == 0) {
        double pi_estimate = 4 * ((double)number_in_circle / (double)number_of_tosses);
        cout << "Estimacion de Pi = " << pi_estimate << endl;
    }

    MPI_Finalize();
    return 0;
}