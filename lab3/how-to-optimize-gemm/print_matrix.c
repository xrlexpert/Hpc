#include <stdio.h>

#define A( i, j ) a[ (j)*lda + (i) ]

void print_matrix( int m, int n, double *a, int lda )
{
  int i, j;

  for ( j=0; j<n; j++ ){
    for ( i=0; i<m; i++ )
      printf("%le ", A( i,j ) );
    printf("\n");
  }
  printf("\n");
}


void print_row_matrix( int m, int n, double *a, int lda )
{
  int i, j;
  // printf("{\n");
  for ( i=0; i<m; i++ ){
    for ( j=0; j<n; j++ ){
      printf("%le ", A( i,j ) );
      // printf("%le, ", A( i,j ) );
    }
    printf("\n");
  }
  // printf("}\n");
  printf("\n");
}
