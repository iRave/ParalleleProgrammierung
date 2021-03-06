#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <sys/time.h>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define MAT_SIZE 2048
#define ACCURACY 100
#define RUN_COUNT 10

void init(void);
double matMult(void);
void printMatrix(void);
double getTime(void);

float leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];
int main()
{
    double singleTime = 0;
    double averageTime = 0;
    int i;
    init();
#ifdef DEBUG
    LOG("leftMat:\n");
    printMatrix(leftMat);
    LOG("\nRigthMat:\n");
    printMatrix(rigthMat);
#endif
  for(i = 0; i < RUN_COUNT; i++)
  {
    singleTime = getTime();
    matMult();
    averageTime += getTime() - singleTime;
  }
    printf("Time: %.8lf\n", averageTime/RUN_COUNT);
#ifdef DEBUG
    LOG("\nresultMat:\n");
    printMatrix(resultMat);
#endif
}

void init()
{
    srand(time(NULL));
    int i,j;
    for(i = 0; i < MAT_SIZE; i++)
    {
        for(j = 0; j < MAT_SIZE; j++)
        {
            leftMat[i][j] = ((float)(rand() % ACCURACY))/ACCURACY;
            rigthMat[i][j] = ((float)(rand() % ACCURACY))/ACCURACY;
            resultMat[i][j] = 0.0;
        }
    }
}

double matMult()
{
    for (int i = 0; i < MAT_SIZE; i++){
        for (int j = 0; j < MAT_SIZE; j++) {
            for (int k = 0; k < MAT_SIZE; k++) {
                resultMat[i][j] += leftMat[i][k] * rigthMat[k][j];
            }
        }
    }

}

double getTime(){  /* elapsed time in ms */
    struct timeval curr;
    gettimeofday(&curr, NULL);
    double tmp = (double)curr.tv_sec * 1000.0 + (double)curr.tv_usec/1000.0;
    return tmp;
}

void printMatrix()
{
    int i,j;
    for (i = 0; i < MAT_SIZE; i++)
    {
        for (j = 0; j < MAT_SIZE; j++)
        {
            LOG("| %.2f", matrix[i][j]);
        }
        LOG("\n");
    }
}
