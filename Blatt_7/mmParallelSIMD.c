#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h>
#include<immintrin.h>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define MAT_SIZE 2048
#define ACCURACY 100
#define RUN_COUNT 100 

void init(void);
double matMult(void);
void printMatrix(float matrix[][MAT_SIZE]);
double getTime(void);

__attribute__((aligned(32)))float leftMat[MAT_SIZE][MAT_SIZE], rigthMat[MAT_SIZE][MAT_SIZE], resultMat[MAT_SIZE][MAT_SIZE];
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
    #pragma omp target device(0) map(to:leftMat,rigthMat) map(from:resultMat)
    {
    __m256 summ, leftElements, rigthElements;
    __attribute__((aligned(32))) float temp[8];
    omp_set_dynamic(0);
    omp_set_num_threads(THREAD_COUNT);
    #pragma omp parallel for private(temp, summ, leftElements, rigthElements)
    for (int i = 0; i < MAT_SIZE; i++){
        //printf("ThreadID: %d\n", omp_get_thread_num());
        for (int j = 0; j < MAT_SIZE; j++) {
            summ = _mm256_setzero_ps();
            for (int k = 0; k < MAT_SIZE; k+=8) {
                leftElements = _mm256_load_ps(&leftMat[i][k]);
                rigthElements = _mm256_load_ps(&rigthMat[j][k]);                
                summ = _mm256_add_ps(summ, _mm256_mul_ps(leftElements, rigthElements));
            }
            _mm256_storeu_ps(temp, summ);
            for(int k = 0; k < 8; k++) {
                resultMat[i][j] += temp[k];
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

void printMatrix(float matrix[][MAT_SIZE])
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
