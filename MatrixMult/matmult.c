#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>                     
#define DIM 10
#define THREAD_COUNT 10

double X[DIM][DIM], M[DIM][DIM], Y[DIM][DIM];
typedef struct{int rows; int rowCount;} work_t;
void* matmult(void* work) {        
  int i,row, col;
  for (row = 0; row < work->rowCount; row++){
    for (col = 0; col < DIM; col++) {
      Y[work->rows[row]][col]=0;
      for (i=0; i < DIM; i++)
        Y[work->rows[row]][col] += X[work->rows[row]][i] * M[i][col];
    }
  }
}

int main() {
  int i,j;
  int lines[DIM / THREAD_COUNT + 1];
  work_t work[DIM][DIM];
  pthread_t tid[THREAD_COUNT];

  for (i=0; i<DIM; i++)
    for (j=0; j < DIM; j++) {
      X[i][j] = drand48(); M[i][j] = drand48();
    }

  for (i=0; i<DIM; i++)
    for (j=0; j < DIM; j++) {
      work[i][j].row = i; work[i][j].col = j; 
      pthread_create(&tid[i][j], NULL, matmult,(void*) &work[i][j]);
    }
  for (i=0; i<DIM; i++)
    for (j=0; j < DIM; j++) 
      pthread_join(tid[i][j], NULL);

  for (i=0; i<DIM; i++) {
    for (j=0; j < DIM; j++) 
      printf("%f ", Y[i][j]);
    printf("\n"); 
  }
}
