#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv) {
        int size;
        int rank;
        int transferNum = 100000;
	int i;
        int j;
	int n = 6;

        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

  	for (j = 0; j < n; j++) {
		int power = pow(10, j);
		int num[power];
		if (rank == 0) {
			clock_t start = 1000 * MPI_Wtime();
			for (i = 0; i < transferNum; i++) {
				MPI_Send(num, power, MPI_INT, 1, 5, MPI_COMM_WORLD);
				MPI_Recv(num, power, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
			}
			clock_t end = 1000 * MPI_Wtime();
			double seconds = (double)(end - start);
			printf("%f микросекунд на коммуникацию между узлами буфера размером в %d элементов\n", seconds * 1000 / (2.0 * transferNum), power);
		}	

		if (rank == 1) {
                	for (i = 0; i < transferNum; i++) {
                        	MPI_Recv(num, power, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
				MPI_Send(num, power, MPI_INT, 0, 5, MPI_COMM_WORLD);
                	}
        	}
	}

        MPI_Finalize();
        return 0;
}

