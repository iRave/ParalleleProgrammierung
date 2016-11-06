//
//  main.c
//  MatMult
//
//  Created by Till Busse on 05/11/16.
//  Copyright © 2016 Till Busse. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define DIM 100
#define THREAD_COUNT 10
#define MAX_ROW_PER_THREAD DIM/10+1

double X[DIM][DIM], M[DIM][DIM], Y[DIM][DIM];
typedef struct{
  int rows[MAX_ROW_PER_THREAD];
  int rowCount;
} work_t;

void matmult(void* arg)
{
  int i,row, col;
  work_t *work = (work_t*)arg;
  for (row = 0; row < work->rowCount; row++)
  {
    for (col = 0; col < DIM; col++)
    {
      Y[work->rows[row]][col]=0;
      for (i=0; i < DIM; i++)
        Y[work->rows[row]][col] += X[work->rows[row]][i] * M[i][col];
    }
  }
}

int main() {
  int i,j;
  work_t work[THREAD_COUNT];
  pthread_t tid[THREAD_COUNT];

  for (i=0; i<DIM; i++)
  {
    for (j=0; j < DIM; j++)
    {
      X[i][j] = drand48(); M[i][j] = drand48();
    }
  }

  for (j=0; j < THREAD_COUNT; j++)
  {
    work[j].rowCount = 0;
  }


  for (i=0; i<DIM; i++)
  {
    int currentThread = (i % THREAD_COUNT);
    work[currentThread].rows[work[currentThread].rowCount] = i;
    work[i % THREAD_COUNT].rowCount++;
  }

  for (j=0; j < THREAD_COUNT; j++)
  {
    pthread_create(&tid[j], NULL, (void*)matmult,(void*) &work[j]);
  }

  for (j=0; j < THREAD_COUNT; j++)
  {
    pthread_join(tid[j], NULL);
  }

  for (i=0; i<DIM; i++)
  {
    for (j=0; j < DIM; j++)
    {
      printf("%f ", Y[i][j]);
    }
    printf("\n");
  }
}
