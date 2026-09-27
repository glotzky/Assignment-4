#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "matrix.h"


float SJF(int* jobs, int size) {
    if (jobs == NULL || size <= 0) return 0.0f; 
    for (int i = 0; i < size - 1; i++) { 
        for (int j = i + 1; j < size; j++) { 
            if (jobs[i] > jobs[j]) { 
            int temp = jobs[i]; 
            jobs[i] = jobs[j]; 
            jobs[j] = temp; } 
        }
    }

    double elapsed = 0.0; 
    double total = 0.0; 
    for (int i = 0; i < size; i++) { 
        double n = jobs[i]; 
        elapsed += n * n * n; 
        total += elapsed; 
    } 
    return (float)(total / size); 
}


float FIFO(int* jobs, int size) {
	if (jobs == NULL || size <= 0) return 0.0f;
	double elapsed = 0.0;
	double total = 0.0;
	for (int i = 0; i < size; i++) {
		double n = jobs[i];
		elapsed += n * n * n;
		total += elapsed;
	}
	return (float)(total / size);
}
