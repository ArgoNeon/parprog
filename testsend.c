#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

double iter_pi(int i) {
	double res = pow((-1.0), i) / (2 * i + 1);
	return res;
}

int main(int argc, char **argv) {
	int size;
    	int rank;
   	double pi = 0.0;
	int last = 1000000;
	int i;
	int j;

    	MPI_Status status;
    	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);

    	printf("Number of threads: %d\n", size);    

    	for (i = 0; i < size; i++) {
    		if (rank == i) {
			j = i;
			while (j < last) {
				pi = pi + iter_pi(j);
				j = j + size;
			}
			printf("pi: %f rank: %d\n", pi, rank);
		}
    	}

    	if (rank != 0) {
		MPI_Send(&pi, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    	}

    	if (rank == 0) {
		double recv;
	    	for (i = 1; i < size; i++) {
			MPI_Recv(&recv, 1, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &status);
			pi = pi + recv;
	    	}
		pi = 4 * pi;
	    	printf("pi: %f\n", pi);
    	}
	
	MPI_Finalize();
	return 0;
}

    /*if(rank == 0)
    {
	for (i = 0; i < accuracy)
        MPI_Send(&randlen, 1, MPI_INT, 1, 5, MPI_COMM_WORLD);
        MPI_Send(buf, randlen, MPI_CHAR, 1, 5, MPI_COMM_WORLD);
        free(buf);
    }
    if(rank == 1)
    {
        int randlen;
        MPI_Recv(&randlen, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
        void *buf = malloc(randlen * sizeof(char) + 1);
        MPI_Recv(buf, randlen, MPI_CHAR, 0, 5, MPI_COMM_WORLD, &status);

        *((char*)buf + randlen) = '\0';
        printf("%s\n", buf);
        free(buf);
    }
    MPI_Finalize();
}*/
