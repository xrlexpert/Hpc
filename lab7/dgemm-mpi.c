#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4
double a[N][N]={
-1.000000e+00, -2.707955e-01, 5.350056e-02, 8.634630e-01, 
-9.980292e-01, -8.173388e-01, -9.113315e-02, 1.361192e-01, 
-9.167380e-01, -8.154047e-01, -5.336431e-01, 1.121887e-01, 
-6.467147e-01, -2.556555e-02, 6.625836e-01, -8.983362e-01, 
};
double b[N][N]={
5.341023e-01, 7.519617e-01, 6.208589e-01, -8.464509e-01, 
-9.621704e-01, 6.311373e-02, -6.231595e-01, 6.305478e-01, 
-4.952805e-01, 8.405219e-01, 7.726289e-01, 9.697820e-01, 
-4.036057e-01, 3.086230e-02, 1.412280e-01, -7.632966e-01, 
};
void matMulti(double *A, double *B, double*C, int row, int n){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < n; j++){
            C[i*n + j] = 0;
            for (int k = 0; k < n; k++) 
                C[i*n + j] += A[i*n + k] * B[k*n + j];
        }
    }
}
int main(int argc, char *argv[]) {
    int n=N;
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    printf("%d\n",size);
    int each_row=n/(size-1);
    // 主进程初始化矩阵 A 和 B
    if (rank == 0) {
        // 初始化矩阵 A 和 B
        // ...
        double *A = (double *)malloc(N * N* sizeof(double));
        double *B = (double *)malloc(N * N* sizeof(double));
        double *C = (double *)malloc(N * N* sizeof(double));

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                A[i*N+j]=a[i][j];
                B[i*N+j]=b[i][j];
            }
        }
        for(int i=0;i<size-1;i++)
        {
            int begin_row=each_row*i;
            int end_row=each_row*(i+1);
            MPI_Send(&A[begin_row*N+0],each_row*N,MPI_DOUBLE,i+1,1,MPI_COMM_WORLD);
            MPI_Send(&B[0 * N+ 0], N * N, MPI_DOUBLE, i + 1, 2, MPI_COMM_WORLD);
        }
        for (int i = 0; i < size-1; i++){
         	int begin_row=each_row*i;
            MPI_Recv(&C[begin_row * n + 0], each_row*n, MPI_DOUBLE, i + 1, 3, MPI_COMM_WORLD, &status);
        }
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                printf("%lf ",C[i*n+j]);
            }
            printf("\n");
        }
    }
    else
    {
        double* A = (double *)malloc(each_row * N* sizeof(double)); // A[beginRow:endRow, :]
        double* B = (double *)malloc(N * N* sizeof(double));
        double* C= (double *)malloc(each_row * N* sizeof(double)); //
        MPI_Recv(&A[0 * n + 0], each_row * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);//从A[0][0]和B[0][0]开始接受
        MPI_Recv(&B[0 * n + 0], n * n, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
        matMulti(A, B, C, each_row, n);
        MPI_Send(&C[0 * n + 0], each_row*n, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}
