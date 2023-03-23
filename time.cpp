#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv) {
        int size;
        int rank;
        int transferNum = 1000000;
        int num = 0;
	int i;
        int j;

        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

  	if (rank == 0) {
		clock_t start = clock();
		for (i = 0; i < transferNum; i++) {
			MPI_Send(&num, 1, MPI_INT, 1, 5, MPI_COMM_WORLD);
			MPI_Recv(&num, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
		}
		clock_t end = clock();
		long double seconds = (long double)(end - start) / CLOCKS_PER_SEC;
		printf("%Lf\n", seconds);
		printf("%Lf микросекунд на коммуникацию между узлами\n", seconds * 1000000 / (2.0 * transferNum));
	}	

	if (rank == 1) {
                for (i = 0; i < transferNum; i++) {
                        MPI_Recv(&num, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
			MPI_Send(&num, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
                }
        }

        MPI_Finalize();
        return 0;
}

