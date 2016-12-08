#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define MAT_SIZE 5
#define MAX_RAND 10

void init(double leftMat[MAT_SIZE][MAT_SIZE], double rightMat[MAT_SIZE][MAT_SIZE]);
void matMult(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE]);
void printMatrix(double matrix[MAT_SIZE][MAT_SIZE]);

int main()
{
  double leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];

  init(leftMat, rigthMat);
  printf("leftMat:\n");
  printMatrix(leftMat);
  printf("\nRigthMat:\n");
  printMatrix(rigthMat);
  matMult(leftMat, rigthMat, resultMat);
  printf("\nresultMat:\n");
  printMatrix(resultMat);
}

void init(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE])
{
  srand(time(NULL));
  for(int i = 0; i < MAT_SIZE; i++)
  {
    for(int j = 0; j < MAT_SIZE; j++)
    {
      leftMat[i][j] = (double)(rand() % MAX_RAND);
      rigthMat[i][j] = (double)(rand() % MAX_RAND);
    }
  }
}

void matMult(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE])
{
  for(int i = 0; i < MAT_SIZE; i++)
  {
    for(int j = 0; j < MAT_SIZE; j++)
    {
      for(int k = 0; k < MAT_SIZE; k++)
      {
        resultMat[i][j] += leftMat[i][k] * rigthMat[k][j];
      }
    }
  }
}

void printMatrix(double matrix[MAT_SIZE][MAT_SIZE])
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    for (int j = 0; j < MAT_SIZE; j++)
    {
      printf("| %.2f", matrix[i][j]);
    }
    printf("\n");
  }
}
