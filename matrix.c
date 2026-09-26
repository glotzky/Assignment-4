#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generate_random_matrix(int rows, int cols, int *matrix) {
 for (int row = 0; row<rows; row++){
    for (int col = 0; col<cols; col++){
        matrix[row*cols+col] = rand();
    }
 }
}

void multiply_matrices(int rows1, int cols1, int *matrix1,
                       int rows2, int cols2, int *matrix2,
                       int *result) {
    for (int row = 0; row < rows1; row++) {
        for (int col = 0; col < cols2; col++) {
            int sum = 0;
            for (int inner = 0; inner < cols1; inner++) {
                sum += matrix1[row * cols1 + inner]
                    * matrix2[inner * cols2 + col];
            }
            result[row * cols2 + col] = sum;
        }
    }
}

void display_matrix(int rows, int cols, int *matrix) {
   for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%d ", matrix[row * cols + col]);
        }
        printf("\n");
    }
}

int do_job(int rows1, int cols1, int cols2, int forever) {
    int *matrix1 = malloc(rows1 * cols1 * sizeof(int));
    int *matrix2 = malloc(cols1 * cols2 * sizeof(int));
    int *result = malloc(rows1 * cols2 * sizeof(int));

    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        free(matrix1);
        free(matrix2);
        free(result);
        return -1;
    }

    printf("Generating Matrices...");
    generate_random_matrix(rows1, cols1, matrix1);
    printf("Matrix 1 done.\n");
    generate_random_matrix(cols1, cols2, matrix2);
    printf("Matrix 2 done.\n");

    do {
        multiply_matrices(rows1, cols1, matrix1,
                          cols1, cols2, matrix2, result);
    } while (forever);

    free(matrix1);
    free(matrix2);
    free(result);
    return 0;
}

