#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

#define MATRIX_SIZE 16
#define SMALL_MATRIX_SIZE 8
#define MATRIX_SIZE_IN_SMALL_MATRIX 2

int main(int argc, char **argv) {
	int smallMatrix[SMALL_MATRIX_SIZE][SMALL_MATRIX_SIZE];
	int tsmallMatrix[SMALL_MATRIX_SIZE][SMALL_MATRIX_SIZE]; 
	int size;
    	int rank;
	int i;
	int j;
	int k;
	int trank;
	int n = MATRIX_SIZE;
	int bufSize = SMALL_MATRIX_SIZE * SMALL_MATRIX_SIZE;
	int line;
        int column;
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

	if (rank == 0) {
		matrix = (int **)calloc(MATRIX_SIZE, sizeof(int *));

                for (i = 0; i < MATRIX_SIZE; i++) {
                        matrix[i] = (int *)calloc(MATRIX_SIZE, sizeof(int));
                }

		for (j = 0; j < MATRIX_SIZE; j++) {
                                for (k = 0; k < MATRIX_SIZE; k++) {
                                        matrix[j][k] = j * MATRIX_SIZE + k;
                                }
                }

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
		for (i = 1; i < size; i++) {
			line = i / MATRIX_SIZE_IN_SMALL_MATRIX;
			column = i % MATRIX_SIZE_IN_SMALL_MATRIX;

			for (j = 0; j < SMALL_MATRIX_SIZE; j++) {
				for (k = 0; k < SMALL_MATRIX_SIZE; k++) {
					smallMatrix[j][k] = matrix[line * SMALL_MATRIX_SIZE + j][column * SMALL_MATRIX_SIZE + k];
				}
			}
			MPI_Send(smallMatrix, bufSize, MPI_INT, i, 5, MPI_COMM_WORLD);
		}
		for (j = 0; j < SMALL_MATRIX_SIZE; j++) {
                                for (k = 0; k < SMALL_MATRIX_SIZE; k++) {
                                        smallMatrix[j][k] = matrix[j][k];
                                }
		}
	}

        if (rank == 0) {
                for (j = 0; j < SMALL_MATRIX_SIZE; j++)
                        for (k = 0; k < SMALL_MATRIX_SIZE; k++)
                                tsmallMatrix[j][k] = smallMatrix[k][j];
	}


	if (rank != 0) {
		MPI_Recv(&smallMatrix, bufSize, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
		for (j = 0; j < SMALL_MATRIX_SIZE; j++)
			for (k = 0; k < SMALL_MATRIX_SIZE; k++)
				tsmallMatrix[j][k] = smallMatrix[k][j];

		MPI_Send(tsmallMatrix, bufSize, MPI_INT, 0, 5, MPI_COMM_WORLD);
	}

        if (rank == 0) {
		for (j = 0; j < SMALL_MATRIX_SIZE; j++) {
                                for (k = 0; k < SMALL_MATRIX_SIZE; k++) {
                                        tmatrix[j][k] = tsmallMatrix[j][k];
                                }
                        }

                for (i = 1; i < size; i++) {
			MPI_Recv(tsmallMatrix, bufSize, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
                        line = i % MATRIX_SIZE_IN_SMALL_MATRIX;
                        column = i / MATRIX_SIZE_IN_SMALL_MATRIX;

                        for (j = 0; j < SMALL_MATRIX_SIZE; j++) {
                                for (k = 0; k < SMALL_MATRIX_SIZE; k++) {
                                        tmatrix[line * SMALL_MATRIX_SIZE  + j][column * SMALL_MATRIX_SIZE + k] = tsmallMatrix[j][k];
                                }
			}
                }
		
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
                	        free(tmatrix[i]);
		}

		for (i = 0; i < MATRIX_SIZE; i++) {
                	        free(matrix[i]);
        	}

		free(tmatrix);
		free(matrix);
	}

	MPI_Finalize();

	return 0;
}
