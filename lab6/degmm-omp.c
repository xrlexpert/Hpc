#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

#define N 1024
#define M 16
#define NUM_THREADS 4

double A[N][N];
double B[N][N];
double C[N][N];

int fmin(int a, int b) {
    if (a < b) return a;
    else return b;
}

// Matrix multiplication function
void dgemm(int n, double *A, double *B, double *C) {
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int ii = 0; ii < n; ii += M) {
        for (int jj = 0; jj < n; jj += M) {
            for (int kk = 0; kk < n; kk += M) {
                for (int i = ii; i < fmin(ii + M, n); i++) {
                    for (int j = jj; j < fmin(jj + M, n); j++) {
                        for (int k = kk; k < fmin(kk + M, n); k++) {
                            C[i * n + j] += A[i * n + k] * B[k * n + j];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // Initialize matrices A and B (for simplicity, assuming random values)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
            B[i][j] = (double)rand() / RAND_MAX;
            C[i][j] = 0.0;
        }
    }

    struct timeval start, finish;
    gettimeofday(&start, NULL);

    dgemm(N, (double *)A, (double *)B, (double *)C);

    gettimeofday(&finish, NULL);
    double duration = ((double)(finish.tv_sec - start.tv_sec) * 1000000 + (double)(finish.tv_usec - start.tv_usec)) / 1000000;
    printf("Total time: %lf seconds\n", duration);

    return 0;
}