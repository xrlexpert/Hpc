#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define N 1024
#define M 16

double A[N][N];
double B[N][N];
double C[N][N];

typedef struct {
    int n;
    double* a;
    double* b;
    double* c;
    int start_row;
    int end_row;
} myarg_t;

int fmin(int a, int b) {
    if (a < b) return a;
    else return b;
}

// Matrix multiplication function
void *dgemm(void *arg) {
    myarg_t *args = (myarg_t *)arg;
    int n = args->n;
    double *C = args->c;
    double *A = args->a;
    double *B = args->b;
    int start_row = args->start_row;
    int end_row = args->end_row;

    for(int i=0;i<n;i++)
    	for(int j=0;j<n;j++)
    		for(int k=0;k<n;k++)
    		{
    			C[i*n+j]+=A[i*n+k]*B[k*n+j];
    		}
    return NULL;
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

    myarg_t args;
    struct timeval start, finish;
    
    
    gettimeofday(&start, NULL);
 
  	pthread_t p1;
  	args.n = N;
        args.a = (double *)A;
        args.b = (double *)B;
        args.c = (double *)C;
        args.start_row = 0;
        args.end_row = N;
   	int rc = pthread_create(&p1, NULL, dgemm, &args);
    	assert(rc == 0);
    
   
   	rc=pthread_join(p1, NULL);
   	assert(rc==0);
    
    gettimeofday(&finish, NULL);
    double duration = ((double)(finish.tv_sec - start.tv_sec) * 1000000 + (double)(finish.tv_usec - start.tv_usec)) / 1000000;
    printf("Total time: %lf seconds\n", duration);

    return 0;
}