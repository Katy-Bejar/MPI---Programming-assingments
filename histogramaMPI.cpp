#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int NUM_BINS = 5;  // Número de intervalos en el histograma
const double MIN_MEAS = 0.0;  // Valor mínimo del rango de datos
const double MAX_MEAS = 5.0;  // Valor máximo del rango de datos

// Calcula el intervalo (bin) para cada valor
int find_bin(double value, const vector<double>& bin_maxes) {
    for (int b = 0; b < NUM_BINS; b++) {
        if (value < bin_maxes[b]) {
            return b;
        }
    }
    return NUM_BINS - 1;  // En caso de que el valor esté en el último bin
}

// Calcula el histograma parcial para un conjunto de datos
void calculate_histogram(const vector<double>& data, int* histogram, const vector<double>& bin_maxes) {
    for (double value : data) {
        int bin = find_bin(value, bin_maxes);
        histogram[bin]++;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int num_elements = 20;  // Número de datos de entrada total
    int num_elements_per_proc = num_elements / world_size;

    // Definir el ancho de cada bin e inicializar los límites superiores de los bins
    double bin_width = (MAX_MEAS - MIN_MEAS) / NUM_BINS;
    vector<double> bin_maxes(NUM_BINS);
    for (int i = 0; i < NUM_BINS; i++) {
        bin_maxes[i] = MIN_MEAS + bin_width * (i + 1);
    }

    // Proceso 0 genera o lee los datos
    vector<double> data;
    if (world_rank == 0) {
        data = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3,
                4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    }

    // Cada proceso recibe una porción de datos
    vector<double> sub_data(num_elements_per_proc);
    MPI_Scatter(data.data(), num_elements_per_proc, MPI_DOUBLE,
                sub_data.data(), num_elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Cada proceso calcula su histograma parcial
    int local_histogram[NUM_BINS] = {0};
    calculate_histogram(sub_data, local_histogram, bin_maxes);

    // El proceso 0 recolecta los histogramas parciales y suma para obtener el histograma final
    int global_histogram[NUM_BINS] = {0};
    MPI_Reduce(local_histogram, global_histogram, NUM_BINS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprime el histograma final en el proceso 0
    if (world_rank == 0) {
        cout << "Histograma final:" << endl;
        for (int i = 0; i < NUM_BINS; i++) {
            if (i == 0) {
                cout << "Intervalo " << i << " (" << MIN_MEAS << " - " << bin_maxes[i] << "): " << global_histogram[i] << endl;
            } else {
                cout << "Intervalo " << i << " (" << bin_maxes[i - 1] << " - " << bin_maxes[i] << "): " << global_histogram[i] << endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}