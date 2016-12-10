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
#define START 230
#define MAX_THREAD_COUNT START+1
#define RUN_COUNT 1

typedef struct SmatmultReturn{
    double execTime;
    double copyTimeTo;
    double copyTimeFrom;
} MatmultReturn;

void init(void);
void matMult(MatmultReturn *ret, int threadCound);
void printMatrix(double **);
#pragma omp declare target
double getTime(void);
#pragma omp end declare target

double leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];

int main()
{
    double execTimes=0, copyFromTimes=0, copyToTimes=0, minTime=DBL_MAX;
    int i,j, fastestThreadCount;
    MatmultReturn matMultTimes;
    omp_set_dynamic(0);
    init();

#ifdef DEBUG
    LOG("leftMat:\n");
    printMatrix(leftMat);
    LOG("\nRigthMat:\n");
    printMatrix(rigthMat);
#endif

    for (i = START; i <= MAX_THREAD_COUNT; i+=10) {
        for (j = 0; j < RUN_COUNT; ++j) {
            //omp_set_num_threads(i);
            matMult(&matMultTimes, i);
            execTimes += matMultTimes.execTime;
            copyToTimes += matMultTimes.copyTimeTo;
            copyFromTimes += matMultTimes.copyTimeFrom;
        }
        execTimes /= RUN_COUNT;
        copyToTimes /= RUN_COUNT;
        copyFromTimes /= RUN_COUNT;
        printf("ThreadCount: %i\nTime: %.8lf\nCopyToTime: %.8lf\nCopyFromTime: %8lf\n", i,execTimes,copyToTimes,copyFromTimes);
        if(execTimes < minTime) {
            minTime = execTimes;
            fastestThreadCount = i;
        }
        execTimes = 0;
        copyToTimes = 0;
        copyFromTimes = 0;
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

void matMult(MatmultReturn *ret, int threadCount) {
    double locExecTime;
    ret->copyTimeTo = getTime();
    //omp_set_num_threads(10);
    #pragma offload_transfer target(mic) in(leftMat,rigthMat, threadCount, resultMat)
        ret->copyTimeTo = getTime() - ret->copyTimeTo;
        #pragma offload target(mic) out(locExecTime)
        {
            int i, j, k;

            #ifdef DEBUG
            char host[80];
            gethostname(host, 80);
            LOG("Running on %s\n", host);
            #endif

            locExecTime = getTime();
            LOG("Local time1: %lf\n",locExecTime);
            
            omp_set_num_threads(threadCount);
            #pragma omp parallel for
            for (i = 0; i < MAT_SIZE; i++) {
        //        printf("ThreadID: %d\n", omp_get_thread_num());
                for (j = 0; j < MAT_SIZE; j++) {
                    for (k = 0; k < MAT_SIZE; k++) {
                        resultMat[i][j] += leftMat[i][k] * rigthMat[k][j];
                    }
                }
            }
            
            locExecTime = getTime() - locExecTime;
            LOG("Local time2: %lf\n",locExecTime);
        }
        ret->copyTimeFrom = getTime();
    #pragma offload_transfer target(mic) out(resultMat)
    ret->copyTimeFrom = getTime() - ret->copyTimeFrom;
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
