#define _BSD_SOURCE

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <sys/time.h>
#include<omp.h>
#include<unistd.h>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define MAT_SIZE 2048
#define ACCURACY 100
#define RUN_COUNT 5

#define MIN_THREAD_COUNT 230
#define MAX_THREAD_COUNT 230
#define STEP_SIZE 5

void init(void);
double matMult(int);
void printMatrix(void);
double getTime(void);

float leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];
int main()
{
    double singleTime = 0;
    double averageTime = 0;
    double minTime = 100000000;
    double optThreadCount = 0;
    init();
#ifdef DEBUG
    LOG("leftMat:\n");
    printMatrix(leftMat);
    LOG("\nRigthMat:\n");
    printMatrix(rigthMat);
#endif
  for(int j = MIN_THREAD_COUNT; j <= MAX_THREAD_COUNT; j += STEP_SIZE)
  {
    double averageTime = 0;
    printf("ThreadCount: %d", j);
    for(int i = 0; i < RUN_COUNT; i++)    
    { 
      singleTime = getTime();
      matMult(j);
      averageTime += getTime() - singleTime;
    }
    averageTime /= RUN_COUNT;
    printf("Time: %.8lf\n", averageTime);
    if(averageTime < minTime)
    {
      minTime = averageTime;
      optThreadCount = j;
    }
  }

  printf("Best: %d Threads with %f s\n", optThreadCount, minTime);
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

double matMult(int threadCount)
{
    #pragma omp target device(0) map(to:leftMat,rigthMat) map(tofrom:resultMat)    
    {
       //char host[80];
       omp_set_dynamic(0);
       omp_set_num_threads(threadCount);
       //gethostname(host, 80);
       //printf("%s\n", host);
       #pragma omp parallel for 
       #pragma omp parallel for simd collapse(3)
       for (int i = 0; i < MAT_SIZE; i++){
            for (int j = 0; j < MAT_SIZE; j++) {
                for (int k = 0; k < MAT_SIZE; k++) {
                    resultMat[i][j] += leftMat[i][k] * rigthMat[k][j];
                }
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
