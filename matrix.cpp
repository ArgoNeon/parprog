#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

#define MATRIX_SIZE 32

int main(int argc, char **argv) {
	int size;
    	int rank;
	int string[MATRIX_SIZE];
	int column[MATRIX_SIZE];
	int i;
	int j;
	int k;
	/*int matrix[MATRIX_SIZE][MATRIX_SIZE] = {	{1,	2, 	3, 	4},
	       						{5,	6, 	7, 	8},
							{9,	10, 	11, 	12},
							{13,	14, 	15, 	16}	};*/
        int** matrix;
	int** tmatrix;

    	MPI_Status status;
    	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);   

	matrix = (int **)calloc(MATRIX_SIZE, sizeof(int *));

        for (i = 0; i < MATRIX_SIZE; i++) {
        	matrix[i] = (int *)calloc(MATRIX_SIZE, sizeof(int));
       	}

	for (j = 0; j < MATRIX_SIZE; j++) {
        	for (k = 0; k < MATRIX_SIZE; k++) {
                	matrix[j][k] = j * MATRIX_SIZE + k;
             	}
     	}

	 if (rank == 0) {
		for (j = 0; j < MATRIX_SIZE; j++) {
                                for (k = 0; k < MATRIX_SIZE; k++) {
                                        printf ("%6d", matrix[j][k]);
                                }

                                printf("\n");
                }

		tmatrix = (int **)calloc(MATRIX_SIZE, sizeof(int *));

		for (i = 0; i < MATRIX_SIZE; i++) {
			tmatrix[i] = (int *)calloc(MATRIX_SIZE, sizeof(int));
		}
	}

	if (rank == 0) {
		for (i = 0; i < MATRIX_SIZE; i++) {
			printf("proc %d i %d\n", (i % (size - 1)) + 1, i);
			MPI_Recv(string, MATRIX_SIZE, MPI_INT, (i % (size - 1)) + 1, 5, MPI_COMM_WORLD, &status);
			for (j = 0; j < MATRIX_SIZE; j++) {
				tmatrix[i][j] = string[j];
			}
		}
	}


	if (rank != 0) {
		for (i = 0; i < MATRIX_SIZE; i++) {
			if (rank == (i % (size - 1)) + 1) {
				for (j = 0; j < MATRIX_SIZE; j++) {
					column[j] = matrix[i][j];
				}
				MPI_Send(column, MATRIX_SIZE, MPI_INT, 0, 5, MPI_COMM_WORLD);
			}
		}
	}

        if (rank == 0) {
		printf("\n");
		for (j = 0; j < MATRIX_SIZE; j++) {
                                for (k = 0; k < MATRIX_SIZE; k++) {
                                        printf ("%6d", tmatrix[j][k]);
				}

				printf("\n");
		}
	}

	if (rank == 0) {
		for (i = 0; i < MATRIX_SIZE; i++) {
         	       free(matrix[i]);
        	}

		for (i = 0; i < MATRIX_SIZE; i++) {
                	        free(tmatrix[i]);
		}

		free(matrix);
		free(tmatrix);
	}

	MPI_Finalize();

	return 0;
}
