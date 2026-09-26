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
    // Allocate A, B, and the result matrix once and reuse them.
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

    float total_time = 0.0f;
    do {
        struct timespec t0, t1;
        // Time only the matrix multiplication, not matrix generation.
        timespec_get(&t0, TIME_UTC);
        multiply_matrices(rows1, cols1, matrix1,
                          cols1, cols2, matrix2, result);
        timespec_get(&t1, TIME_UTC);

        float dns = (float)(t1.tv_nsec - t0.tv_nsec) / 1000000000.0f;
        float ds = (float)(t1.tv_sec - t0.tv_sec);
        total_time = dns + ds;
    } while (forever);

    free(matrix1);
    free(matrix2);
    free(result);
    return (int)(total_time * 1000000.0f);
}

