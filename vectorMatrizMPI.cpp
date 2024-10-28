#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n = 4;  // Orden de la matriz
    int local_cols = n / world_size;  // Columnas asignadas a cada proceso

    vector<double> matrix;
    vector<double> vector_in(n);
    vector<double> local_matrix(n * local_cols);
    vector<double> local_result(n, 0.0);

    // Proceso 0 inicializa la matriz y el vector
    if (world_rank == 0) {
        matrix = {
            1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0
        };
        vector_in = {1.0, 1.0, 1.0, 1.0};
    }

    // Distribuir el vector completo a todos los procesos
    MPI_Bcast(vector_in.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Proceso 0 distribuye columnas de la matriz en bloques a cada proceso
    if (world_rank == 0) {
        for (int p = 1; p < world_size; p++) {
            MPI_Send(&matrix[p * n * local_cols], n * local_cols, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        local_matrix = vector<double>(matrix.begin(), matrix.begin() + n * local_cols);
    } else {
        MPI_Recv(local_matrix.data(), n * local_cols, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Cada proceso calcula su parte del producto matriz-vector
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < local_cols; j++) {
            local_result[i] += local_matrix[i * local_cols + j] * vector_in[j + world_rank * local_cols];
        }
    }

    // Sumar los resultados parciales en el proceso 0
    vector<double> final_result(n);
    MPI_Reduce_scatter(local_result.data(), final_result.data(), &local_cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Proceso 0 imprime el resultado final
    if (world_rank == 0) {
        cout << "Resultado final (producto matriz-vector):" << endl;
        for (double val : final_result) {
            cout << val << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}