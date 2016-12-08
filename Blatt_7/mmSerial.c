#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define MAT_SIZE 10
#define ACCURACY 100

void init(double leftMat[MAT_SIZE][MAT_SIZE], double rightMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE]);
void matMult(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE]);
void printMatrix(double matrix[MAT_SIZE][MAT_SIZE]);

int main()
{
  double leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];

  init(leftMat, rigthMat, resultMat);

  printf("leftMat:\n");
  printMatrix(leftMat);
  printf("\nRigthMat:\n");
  printMatrix(rigthMat);

  matMult(leftMat, rigthMat, resultMat);

  printf("\nresultMat:\n");
  printMatrix(resultMat);
}

void init(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE])
{
  srand(time(NULL));
  for(int row = 0; row < MAT_SIZE; row++)
  {
    for(int col = 0; col < MAT_SIZE; col++)
    {
      leftMat[row][col] = ((double)(rand() % ACCURACY))/ACCURACY;
      rigthMat[row][col] = ((double)(rand() % ACCURACY))/ACCURACY;
      resultMat[row][col] = 0.0;
    }
  }
}

void matMult(double leftMat[MAT_SIZE][MAT_SIZE], double rigthMat[MAT_SIZE][MAT_SIZE], double resultMat[MAT_SIZE][MAT_SIZE])
{
  for(int row = 0; row < MAT_SIZE; row++)
  {
    for(int col = 0; col < MAT_SIZE; col++)
    {
      for(int element = 0; element < MAT_SIZE; element++)
      {
        resultMat[row][col] += leftMat[row][element] * rigthMat[element][col];
      }
    }
  }
}

void printMatrix(double matrix[MAT_SIZE][MAT_SIZE])
{
  for (int row = 0; row < MAT_SIZE; row++)
  {
    for (int col = 0; col < MAT_SIZE; col++)
    {
      printf("| %.2f", matrix[row][col]);
    }
    printf("\n");
  }
}
