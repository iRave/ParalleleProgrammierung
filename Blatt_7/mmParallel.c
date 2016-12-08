#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <float.h>
#include <unistd.h>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define MAT_SIZE 2000
#define ACCURACY 100
#define MAX_THREAD_COUNT 10000
#define RUN_COUNT 10

void init(void);
void matMult(void);
void printMatrix(void);
double getTime(void);

double leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];
int main()
{
    double t1=0, times=0,minTime=DBL_MAX;
    int i,j, fastestThreadCount;

    init();
#ifdef DEBUG
    LOG("leftMat:\n");
    printMatrix(leftMat);
    LOG("\nRigthMat:\n");
    printMatrix(rigthMat);
#endif
    for (i = 2000; i < MAX_THREAD_COUNT; i+=100) {
        for (j = 0; j < RUN_COUNT; ++j) {
            t1 = getTime();
            omp_set_num_threads(i);
            matMult();
            times += getTime() - t1;
        }
        times /= RUN_COUNT;
        printf("ThreadCount: %i\nTime: %.8lf\n", i,times);
        if(times < minTime) {
            minTime = times;
            fastestThreadCount = i;
        }
    }
    printf("Fastest execution took %.2lf ms with %d threads.", minTime, fastestThreadCount);
#ifdef DEBUG
    LOG("\nresultMat:\n");
    printMatrix(resultMat);
#endif
}

void init()
{
    srand(time(NULL));
    int i,j;
    for(i = 0; i < MAT_SIZE; i++) {
        for(j = 0; j < MAT_SIZE; j++) {
            leftMat[i][j] = ((double)(rand() % ACCURACY))/ACCURACY;
            rigthMat[i][j] = ((double)(rand() % ACCURACY))/ACCURACY;
            resultMat[i][j] = 0.0;
        }
    }
}

void matMult()
{
    int i,j,k = 0;
    char host[80];
#pragma omp target data device(0) map(to:leftMat,rigthMat) map(tofrom:resultMat) map(alloc: i,j,k,host)
    {
        gethostname(host, 80);
        printf("Running on %s\n",host);
#pragma omp parallel for
        for (i = 0; i < MAT_SIZE; i++){
            for (j = 0; j < MAT_SIZE; j++) {
                for (k = 0; k < MAT_SIZE; k++) {
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
    for (i = 0; i < MAT_SIZE; i++) {
        for (j = 0; j < MAT_SIZE; j++) {
            LOG("| %.2f", matrix[i][j]);
        }
        LOG("\n");
    }
}
