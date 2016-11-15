/*
 ============================================================================
 Name        : Blatt4_OpenMP_A3.c
 Author      : Till Busse, Florian Ölsner
 Version     :
 Copyright   : Copy all the things.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// Quicksort Algorithm for N numbers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define N 1000
#define MAX_DEPTH 3
#define MAX_DELTA 100

int a[N];

void quickSort(int arr[], int left, int right, int depth) {
  int i = left, j = right;
  int tmp;
  int pivot = arr[(left + right) / 2];
  depth++;
  printf("Ebene: %d\nLeft: %d\nRight: %d\nThreadID: %d\n\n",depth, left, right,omp_get_thread_num());
  /* partition */
  while (i <= j)
  {
    while (arr[i] < pivot) i++;
    while (arr[j] > pivot) j--;
    if (i <= j)
    {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };

  /* recursion */
  if ((depth < MAX_DEPTH) && ((right-left) > MAX_DELTA)) {
#pragma omp parallel
#pragma omp sections
    {
#pragma omp section
      {
        if (left < j) 	quickSort(arr, left, j, depth);
      }
#pragma omp section
      {
        if (i < right)	quickSort(arr, i, right, depth);
      }
    }
  } else
  {
    if (left < j)   quickSort(arr, left, j, depth);
    if (i < right)  quickSort(arr, i, right, depth);
  }

}


int main(int argc, char* argv[])
{
	int i;
	omp_set_nested(1);
  srand ( (unsigned int)time(NULL) );
  printf("Array initial:\n");
  for(i=0; i<N; i++)
  {
    a[i] = rand()%1000;
    printf("%d ",a[i]);
  }
  printf("\n");

  quickSort(a, 0, N-1, 0);

  printf("Array sortiert:\n");
  for(i=0; i<N; i++)
  {
    printf("%d ",a[i]);
  }
  printf("\n");

  return 0;

}
