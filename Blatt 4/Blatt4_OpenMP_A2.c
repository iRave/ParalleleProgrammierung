/*
 ============================================================================
 Name        : Blatt4_OpenMP_A2.c
 Author      : Till Busse, Florian Ölsner
 Version     :
 Copyright   : Copy all the things.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <quadmath.h>
#include <time.h>

#define N 20000000
#define A2 1

// time our execution time
void timeFunc(void (*func)(void)){
	clock_t begin = clock();
	func();
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f seconds\n",time_spent);
}

#if defined(A1)
void piLinear(){
	int i;
	char buffer[128];
	__float128 k = 0, sum = 0, pi = 0;
	for (i = 0; i < N; ++i) {
		k = (i - 0.5) / N;
		sum += 4.0 / (1.0 + k*k);
	}
	pi = sum / N;
	quadmath_snprintf(buffer, sizeof(buffer),"%.36Qg",pi);
	printf("%s\n",buffer);
}
#elif defined(A2)

__float128 sum = 0;
void piParallel(){
	int i;
	char buffer[128];
	__float128 k = 0, /*sum = 0,*/ pi = 0;
#pragma omp parallel for private(k) reduction(+: sum)
	for (i = 0; i < N; ++i) {
		k = (i - 0.5) / N;
		sum += 4.0 / (1.0 + k*k);
	}
	pi = sum / N;
	quadmath_snprintf(buffer, sizeof(buffer),"%.36Qg",pi);
	printf("%s\n",buffer);
}
#elif defined(A3)
#endif


int main(void) {
#if defined(A1)
	timeFunc(&piLinear);
#elif defined(A2)
	timeFunc(&piParallel);
#elif defined(A3)
#endif
	return EXIT_SUCCESS;
}
