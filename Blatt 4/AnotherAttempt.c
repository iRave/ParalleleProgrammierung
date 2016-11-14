// Quicksort Algorithm for N numbers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#define N 1000

int a[N];
int recCount = 0;

void quickSort(int arr[], int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

  omp_set_num_threads(2);
  #pragma omp parallel shared(arr,pivot,left,right,i,j) private(tmp) if()
  {
    while (i <= j) {
      #pragma omp barrier
      #pragma omp sections
      {
        #pragma omp section
        {
          while (arr[i] < pivot)
          {
            i++;
          }
        }
        #pragma omp section
        {
          while (arr[j] > pivot)j--;
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
          quickSort(arr, left, j);
        }

      }
      #pragma omp section
      {
        if (i < right)
        {
          quickSort(arr, i, right);
        }
      }
    }
  }
}


int main(int argc, char* argv[]) {
  int i;
  omp_set_nested(1);
	srand ( (unsigned int)time(NULL) );

  printf("Array initial:\n");
	for(i=0; i<N; i++) {
		a[i] = rand()%1000;
		printf("%d ",a[i]);
	}
	printf("\n");
  quickSort(a, 0, N-1);

	printf("Array sortiert:\n");
	for(i=0; i<N; i++) {
		printf("%d ",a[i]);
	}
	printf("\n");

	return 0;

}
