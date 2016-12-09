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

#define MAT_SIZE 20
#define ACCURACY 100
#define MAX_THREAD_COUNT 71
#define RUN_COUNT 10

typedef struct SmatmultReturn{
    double execTime;
    double copyTime;
} MatmultReturn;

void init(void);
void matMult(MatmultReturn *ret);
void printMatrix(double **);
#pragma omp declare target
double getTime(void);
#pragma omp end declare target

double leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];
int main()
{
    double execTimes=0,copyToTimes=0,minTime=DBL_MAX;
    int i,j, fastestThreadCount;
    MatmultReturn matMultTimes;

    init();
#ifdef DEBUG
    LOG("leftMat:\n");
    printMatrix(leftMat);
    LOG("\nRigthMat:\n");
    printMatrix(rigthMat);
#endif
    for (i = 70; i < MAX_THREAD_COUNT; i+=10) {
        for (j = 0; j < RUN_COUNT; ++j) {
            omp_set_num_threads(i);
            matMult(&matMultTimes);
            execTimes += matMultTimes.execTime;
            copyToTimes += matMultTimes.copyTime;
        }
        execTimes /= RUN_COUNT;
        copyToTimes /= RUN_COUNT;
        printf("ThreadCount: %i\nTime: %.8lf\nCopyToTime: %.8lf\n", i,execTimes,copyToTimes);
        if(execTimes < minTime) {
            minTime = execTimes;
            fastestThreadCount = i;
        }
    }
    printf("Fastest execution took %.2lf ms with %d threads.\n", minTime, fastestThreadCount);
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

void matMult(MatmultReturn *ret) {
    double locExecTime;
    ret->copyTime = getTime();
#pragma omp target data device(0) map(to:leftMat,rigthMat) map(tofrom:resultMat)
    {
        ret->copyTime = getTime() - ret->copyTime;
#pragma omp target device(0) map(from:locExecTime)
        {
            int i, j, k;
#ifdef DEBUG
            char host[80];
            gethostname(host, 80);
            LOG("Running on %s\n", host);
#endif
            locExecTime = getTime();
            LOG("Local time1: %lf\n",locExecTime);
#pragma omp parallel for
            for (i = 0; i < MAT_SIZE; i++) {
                for (j = 0; j < MAT_SIZE; j++) {
                    for (k = 0; k < MAT_SIZE; k++) {
                        resultMat[i][j] += leftMat[i][k] * rigthMat[k][j];
                    }
                }
            }
            locExecTime = getTime() - locExecTime;
            LOG("Local time2: %lf\n",locExecTime);
        }
    }
    LOG("Returned time from mic0: %lf\n",locExecTime);
    ret->execTime = locExecTime;
}

double getTime(){  /* elapsed time in ms */
    struct timeval curr;
    gettimeofday(&curr, NULL);
    double tmp = (double)curr.tv_sec * 1000.0 + (double)curr.tv_usec/1000.0;
    return tmp;
}

void printMatrix(double **matrix)
{
    int i,j;
    for (i = 0; i < MAT_SIZE; i++) {
        for (j = 0; j < MAT_SIZE; j++) {
            LOG("| %.2f", matrix[i][j]);
        }
        LOG("\n");
    }
}
