// Quicksort Algorithm for N numbers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

int a[N];

void quickSort(int arr[], int left, int right) {
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
            }
	};
 
	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}


int main(int argc, char* argv[]) {  
    int i;

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
