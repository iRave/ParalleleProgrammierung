// Quicksort Algorithm for N numbers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#define N 100000000
#define MAX_DEPTH 20
#define MIN_ARRAY_SIZE 100
#define MEGA_PARALLEL 0

int a[N];
void quickSort(int arr[], int left, int right, int depth)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];
	int parallelismCondition = (depth < MAX_DEPTH)&&((right-left) > MIN_ARRAY_SIZE);
#if (MEGA_PARALLEL == 1)
int parallelismCondition = (depth < MAX_DEPTH)&&((right-left) > MIN_ARRAY_SIZE);
#pragma omp parallel shared(arr,pivot,left,right,i,j) private(tmp) \
  if(parallelismCondition)
  {
    while (i <= j) {
#pragma omp barrier
#pragma omp sections
      {
#pragma omp section
        {
          while (arr[i] < pivot) i++;
        }
#pragma omp section
        {
          while (arr[j] > pivot) j--;
        }
      }
#pragma omp single
      {
        if (i <= j) {
          tmp = arr[i];
          arr[i] = arr[j];
          arr[j] = tmp;
          i++;
          j--;
        }
      }
    }
#pragma omp sections
    {
#pragma omp section
      /* recursion */
      {
        if (left < j)
        {
          quickSort(arr, left, j, ++depth);
        }

      }
#pragma omp section
      {
        if (i < right)
        {
          quickSort(arr, i, right, ++depth);
        }
      }
    }
  }
#else
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
   }

#pragma omp parallel shared(arr,pivot,left,right,i,j) private(tmp) \
  if(parallelismCondition)
    {
#pragma omp sections
    {
#pragma omp section
      /* recursion */
      {
        if (left < j)
        {
          quickSort(arr, left, j, ++depth);
        }

      }
#pragma omp section
      {
        if (i < right)
        {
          quickSort(arr, i, right, ++depth);
        }
      }
    }
  }
#endif
}

int main(int argc, char* argv[]) {
  int i;
  omp_set_nested(1);
  omp_set_num_threads(2);
	srand ( (unsigned int)time(NULL) );
//  printf("Array initial:\n");
	for(i=0; i<N; i++) {
		a[i] = rand()%1000;
	//	printf("%d ",a[i]);
	}
	//printf("\n");
  quickSort(a, 0, N-1, 0);
	//printf("Array sortiert:\n");
	for(i=0; i<N; i++) {
	//	printf("%d ",a[i]);
	}
	printf("\n");
	return 0;

}
