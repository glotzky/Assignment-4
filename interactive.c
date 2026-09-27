#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "matrix.h"
#include "scheduler.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <FIFO/SJF> <job_sizes_comma_separated> \n", argv[0]);
        return 1;
    }
    
    // Select SJF when input otherwise FIFO
    int policy_sjf = strcmp(argv[1], "SJF") == 0;
    // Reject other policies
    if (!policy_sjf && strcmp(argv[1], "FIFO") != 0) {
        printf("Choose FIFO or SJF.\n");
        return 1;
    }

    // The number of jobs is one plus the number of separators in the list
    int job_count = 1;
    // Count commas first so the array can hold every requested matrix size
    for (int char_idx = 0; argv[2][char_idx] != '\0'; char_idx++) {
        if (argv[2][char_idx] == ',') job_count++;
    }

    // Store parsed sizes for the scheduler and future job executions
    int *jobs = malloc((size_t)job_count * sizeof(int));
    // Stop if there is not enough memory to store the request job list
    if (jobs == NULL) {
        printf("Could not allocate job list.\n");
        return 1;
    }

    // Read directly from argv
    char *job_text = argv[2];
    // Parse one positive matrix dimension for each job
    for (int job_idx = 0; job_idx < job_count; job_idx++) {
        int job_size = 0;
        while (*job_text >= '0' && *job_text <= '9') {
            int digit = *job_text - '0';
            if (job_size > (INT_MAX - digit) / 10) {
                printf("Job size too large.\n");
                free(jobs);
                return 1;
            }
            job_size = job_size * 10 + digit;
            job_text++;
        }

        //Positive size required with the expected separator and safe allocation
        if (job_size <= 0 || *job_text != (job_idx == job_count - 1 ? '\0' : ',') ||
            job_size > INT_MAX / sizeof(int) / job_size) {
            printf("Invalid or too-large job size.\n");
            free(jobs);
            return 1;
        }
        // Save dimension
        jobs[job_idx] = job_size;
        // Move past the separator before parsing the next job size
        if (*job_text == ',') job_text++;
    }

    // FIFO keeps the input order; SJF sorts jobs by size and returns the estimate
    float avg_response = policy_sjf ? SJF(jobs, job_count) : FIFO(jobs, job_count);
    //Execute each job in the selected policy order
    for (int job_idx = 0; job_idx < job_count; job_idx++) {
        // Identify the next job
        printf("Running job %d/%d (size %d)\n", job_idx + 1, job_count, jobs[job_idx]);
        // Generate and multiply two matrices
        if (do_job(jobs[job_idx], jobs[job_idx], jobs[job_idx], 0) < 0.0f) {
            // Report matrix allocation failure and release the job
            printf("Could not run job of size %d.\n", jobs[job_idx]);
            free(jobs);
            return 1;
        }
    }

    printf("Average response time: %.2f\n", avg_response);
    free(jobs);
    return 0;
}