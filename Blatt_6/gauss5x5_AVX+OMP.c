#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <immintrin.h>
#include <omp.h>


#define SIZE 1024
#define INTRINSIC_COUNT 8

#ifndef FILTER_COUNT
#define FILTER_COUNT 10
#endif

#ifndef RUN_COUNT
#define RUN_COUNT 5
#endif

#ifndef THREAD_COUNT
#define THREAD_COUNT 8
#endif

//#define DEBUG
#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif
const __attribute__((aligned(32)))float Gauss5x5[5][5] = {
        { 0.0005 , 0.005  , 0.011 , 0.005 , 0.0005 },
        { 0.005  , 0.052  , 0.115 , 0.052 , 0.005  },
        { 0.011  , 0.115  , 0.250 , 0.115 , 0.011  },
        { 0.005  , 0.052  , 0.115 , 0.052 , 0.005  },
        { 0.0005 , 0.005  , 0.011 , 0.005 , 0.0005 }};

float Image[SIZE][SIZE];

void Filter(float Image[SIZE][SIZE], const float Kernel[5][5], int runCount) {
    float tmpImage[SIZE][SIZE];
    __attribute__((aligned(32))) float helpArray[5][5][INTRINSIC_COUNT];
    __m256 kernelVec, envRow, resultRow, summ;
    __attribute__((aligned(32)))float result[INTRINSIC_COUNT];
#pragma omp parallel shared(helpArray, Image, Kernel) private(kernelVec, envRow, resultRow, summ) num_threads(THREAD_COUNT)
    {
        if (omp_get_thread_num() == 1 && runCount == 0)
            LOG("Threadcount: %i\n", omp_get_num_threads());
#pragma omp for
        for (int k = 0; k < 5; k++) {
            for (int l = 0; l < 5; l++) {
                for (int m = 0; m < INTRINSIC_COUNT; m++) {
                    helpArray[k][l][m] = Kernel[k][l];
                }
            }
        }
#pragma omp for
        for (int i = 2; i < SIZE - 2; i++) {
            LOG("ThreadID: %i\n", omp_get_thread_num());
            for (int j = 2; j < SIZE - 2; j += 8) {
                memset(result, 0, INTRINSIC_COUNT);
                summ = _mm256_load_ps(result);
                for (int k = 0; k < 5; k++) {
                    for (int l = 0; l < 5; l++) {
                        kernelVec = _mm256_load_ps(&helpArray[k][l][0]);
                        envRow = _mm256_loadu_ps(&Image[i + k - 2][j + l - 2]);
                        resultRow = _mm256_mul_ps(kernelVec, envRow);
                        summ = _mm256_add_ps(summ, resultRow);
                    }
                }
                _mm256_storeu_ps(&tmpImage[i][j], summ);
            }
        }
#pragma omp for
        for (int h = 2; h < SIZE - 2; h++) {
            for (int b = 2; b < SIZE - 2; b++)
                Image[h][b] = tmpImage[h][b];
        }
    }
}

void WriteImage(float Image[SIZE][SIZE], char* file) {
    int i,j;
    FILE* fd = fopen(file, "w+b");
    fprintf(fd, "P5 ");
    fprintf(fd, "%d %d ", SIZE, SIZE);
    fprintf(fd, "%d ", 255);   /* Max. Grauwert */
    for(i=0;i<SIZE;i++) {
        for(j=0;j<SIZE;j++)
            fprintf(fd,"%c",(unsigned char) Image[i][j]);
    }
    fclose(fd);
}


void ReadImage(char* file, float Image[SIZE][SIZE]) {
    int i,j;
    char version[3];
    int zeilen, spalten, maxgrauwert;
    FILE* fd = fopen(file, "rb");
    fgets(version, sizeof(version), fd);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fd, "%d", &spalten);
    fscanf(fd, "%d", &zeilen);
    fscanf(fd, "%d", &maxgrauwert);
    fgetc(fd);
    for(i=0;i<SIZE;i++) {
        for(j=0;j<SIZE;j++) {
            Image[i][j] = (float)fgetc(fd);
        }
    }
    fclose(fd);
}

double getTime(){  /* elapsed time in ms */
    struct timeval curr;
    gettimeofday(&curr, NULL);
    double tmp = (double)curr.tv_sec * 1000.0 + (double)curr.tv_usec/1000.0;
    return tmp;
}

int main(int argc, char* argv[]) {
    int i;
    double t1=0,t2=0;
    double times = 0;

    for (int j = 0; j < RUN_COUNT; ++j) {
        ReadImage("lena.pgm",Image);
        t1 = getTime();
        for (i=0; i<FILTER_COUNT; i++) {
            Filter(Image, Gauss5x5, j+i);
        }
        t2 = getTime();
        times += (t2 - t1) / FILTER_COUNT;
    }
    times = times / RUN_COUNT;
    WriteImage(Image, "lena_out.pgm");
    printf("result saved in lena_out.pgm\n");
    printf("Ran Algorithm with AVX+OMP %i time(s), filtering the image %i time(s) each.\nAverage execution time: %f ms\n", RUN_COUNT, FILTER_COUNT, times);
}
