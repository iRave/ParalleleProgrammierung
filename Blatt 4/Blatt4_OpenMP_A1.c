/*
 ============================================================================
 Name        : Blatt4_OpenMP.c
 Author      : Till Busse, Florian Ölsner
 Version     :
 Copyright   : Copy all the things.
 Description : Aufgabenblatt 4
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define N 8
#define TREE_HEIGHT 4 // for prefixSum Algorithm
#define A3 1

int i;
#if defined(A1) || defined(A2)
double A[N] = { 0,0,0,0,0 }, B[N] = { 1,1,1,1,1 };
#else
double A[N], B[N];
typedef union treeNode_u
{
  double sum;
  double fromLeft;
}treeNode;
treeNode sumTree[TREE_HEIGHT][N];
#endif
double C[N], D[N], E[N];
double c = 1, x = 1;
double y, sum = 0;

#if defined(A1)
void A1Func(){
//omp_set_num_threads(100);
#pragma omp parallel for shared(D,A,x) private(i,y) reduction(+: sum)
	for (i = 0; i < N; i++) {
	  y = sqrt(A[i]);
	  D[i] = y + A[i] / (x * x);
	  sum += D[i];
	}
	printf("%f",sum);
}
#elif defined(A2)
void A2Func(){
#pragma omp sections
	{
	#pragma omp section
	#pragma omp parallel for shared(A,B,C,c) private(i)
		for (i = 0; i < N; i++) {
			 if (B[i] > 0) {
				 C[i] = x * exp(A[i]) + c * log(B[i]);
				 //printf("C[%d]=%f\n",i,C[i]);
			 }
			 else {
				 C[i] = 0;
			 }
		}
	#pragma omp section
	#pragma omp parallel for shared(A,B,D) private(i)
		for (i = 0; i < N; i++) {
			D[i] = A[i] / (B[i]*B[i]);
		}
	}
#pragma omp parallel for shared(C,D,E,c,x) private(i)
	for (i = 0; i < N; i++) {
		E[i] = c * C[i] + 1/x * D[i];
	}
	for (i = 0; i < N; ++i) {
		printf("%f\n",E[i]);
	}
}
#elif defined(A3)
void testPrint()
{
  int i, j;
  for(i = 0; i < TREE_HEIGHT; i++)
  {
    for(j = 0; j < N; j++)
    {
      printf("%f|",sumTree[i][j].fromLeft);
    }
    printf("\n");
  }
  printf("\n");
}

void init3()
{
  int i, j;
  for(i = 0; i < TREE_HEIGHT-1; i++)
  {
    for(j = 0; j < N; j++)
    {
      sumTree[i][j].sum = 0;
    }
  }

  for(i = 0; i < N; i++)
  {
    A[i] = 0;
    B[i] = i;
    sumTree[TREE_HEIGHT-1][i].sum = i;
  }
}

void bottomUp()
{
  int i, layer;

  for(layer = TREE_HEIGHT-1; layer > 0; layer--)
  {
    for(i = 0; i < pow(2,layer); i += 2)
    {
      sumTree[layer-1][i/2].sum = sumTree[layer][i].sum + sumTree[layer][i+1].sum;
    }
  }
  testPrint();
}

void topDown()
{
  int layer, i;

  sumTree[0][0].fromLeft = 0;

  for(layer = 0; layer < TREE_HEIGHT-1; layer++)
  {
    for(i = 0; i < pow(2,layer); i++)
    {
      sumTree[layer+1][2*i+1].fromLeft = sumTree[layer+1][2*i].sum + sumTree[layer][i].fromLeft;
      sumTree[layer+1][2*i].fromLeft = sumTree[layer][i].fromLeft;
    }
  }
}
void A3Func(){
	init3();
	bottomUp();
	topDown();
	testPrint();
}
#endif
int main(void) {
#ifdef A1
	A1Func();
#elif A2
	A2Func();
#elif A3
	A3Func();
#endif
	return EXIT_SUCCESS;
}

