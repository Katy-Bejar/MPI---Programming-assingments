#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Cada proceso tiene un valor local para sumar
    int local_value = world_rank + 1;
    int sum = local_value;

    // Comprobar la potencia de dos mas cercana
    int nearest_power_of_two = pow(2, static_cast<int>(log2(world_size)));

    if (world_rank >= nearest_power_of_two) {
        MPI_Send(&sum, 1, MPI_INT, world_rank - nearest_power_of_two, 0, MPI_COMM_WORLD);
    } else {
        if (world_rank + nearest_power_of_two < world_size) {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, world_rank + nearest_power_of_two, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value;
        }
        
        // Suma en estructura de arbol para procesos restantes
        for (int step = 1; step < nearest_power_of_two; step *= 2) {
            if (world_rank % (2 * step) == 0) {
                int received_value;
                MPI_Recv(&received_value, 1, MPI_INT, world_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sum += received_value;
            } else if ((world_rank % step) == 0) {
                MPI_Send(&sum, 1, MPI_INT, world_rank - step, 0, MPI_COMM_WORLD);
                break;
            }
        }
    }

    // Proceso 0 imprime el resultado final
    if (world_rank == 0) {
        cout << "Suma global (cualquier número de procesos): " << sum << endl;
    }

    MPI_Finalize();
    return 0;
}