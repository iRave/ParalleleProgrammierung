#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <immintrin.h>
#include <memory.h>

#define SIZE 1024
#define COUNT 1

const __attribute__((aligned(32))) float Gauss5x5[5][5] = {
			{ 0.0005 , 0.005  , 0.011 , 0.005 , 0.0005 },
                       	{ 0.005  , 0.052  , 0.115 , 0.052 , 0.005  },
                       	{ 0.011  , 0.115  , 0.250 , 0.115 , 0.011  },
                       	{ 0.005  , 0.052  , 0.115 , 0.052 , 0.005  },
                       	{ 0.0005 , 0.005  , 0.011 , 0.005 , 0.0005 }
			};

__attribute__((aligned(32))) float Image[SIZE][SIZE];
__attribute__((aligned(32))) float tmpImage[SIZE][SIZE] = {0};
__attribute__((aligned(32))) float resultArray[8];
__attribute__((aligned(32))) float gaussHelp[6]={0.0005, 0.005, 0.011, 0.052, 0.115, 0.250};
__attribute__((aligned(32))) float help[6][8];
__m256 m1, m2, m3, m4;


void Filter(float Image[SIZE][SIZE]) {
    int i,j,b,h,m,n;
    for(m=0;m<6;m++){
        for(n=0;n<8;n++){
            help[m][n]=gaussHelp[m];
        }
    }
    #pragma omp parallel for private(j, m1, m2, m3, m4)
    for(i=2;i<SIZE-2;i++) {
        for(j=2;j<SIZE-2;j+=8) {

            m1 = _mm256_load_ps(&help[0][0]);
            m2 = _mm256_load_ps(&Image[i-2][j-2]);
            m3 = _mm256_loadu_ps(&Image[i-2][j+2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_load_ps(&Image[i+2][j-2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+2][j+2]);
            m3 = _mm256_add_ps (m2, m3);
            m4 = _mm256_mul_ps(m1,m3);

            m1 = _mm256_load_ps(&help[1][0]);
            m2 = _mm256_loadu_ps(&Image[i-2][j-1]);
            m3 = _mm256_loadu_ps(&Image[i-2][j+1]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_load_ps(&Image[i-1][j-2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i-1][j+2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_load_ps(&Image[i+1][j-2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+1][j+2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+2][j-1]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+2][j+1]);
            m3 = _mm256_add_ps (m2, m3);
            m3 = _mm256_mul_ps(m1,m3);
            m4 = _mm256_add_ps (m4, m3);

            m1 = _mm256_load_ps(&help[2][0]);
            m2 = _mm256_loadu_ps(&Image[i-2][j]);
            m3 = _mm256_load_ps(&Image[i][j-2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i][j+2]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+2][j]);
            m3 = _mm256_add_ps (m2, m3);
            m3 = _mm256_mul_ps(m1,m3);
            m4 = _mm256_add_ps (m4, m3);

            m1 = _mm256_load_ps(&help[3][0]);
            m2 = _mm256_loadu_ps(&Image[i-1][j+1]);
            m3 = _mm256_loadu_ps(&Image[i+1][j+1]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+1][j-1]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i-1][j-1]);
            m3 = _mm256_add_ps (m2, m3);
            m3 = _mm256_mul_ps(m1,m3);
            m4 = _mm256_add_ps (m4, m3);


            m1 = _mm256_load_ps(&help[4][0]);
            m2 = _mm256_loadu_ps(&Image[i][j-1]);
            m3 = _mm256_loadu_ps(&Image[i-1][j]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i+1][j]);
            m3 = _mm256_add_ps (m2, m3);
            m2 = _mm256_loadu_ps(&Image[i][j+1]);
            m3 = _mm256_add_ps (m2, m3);
            m3 = _mm256_mul_ps(m1,m3);
            m4 = _mm256_add_ps (m4, m3);

            m1 = _mm256_load_ps(&help[5][0]);
            m2 = _mm256_loadu_ps(&Image[i][j]);
            m3 = _mm256_mul_ps(m1,m2);
            m4 = _mm256_add_ps (m4,m3);
            
            _mm256_storeu_ps(&tmpImage[i][j], m4);
        }
    }

for(h=2;h<SIZE-2;h++) {
        for(b=2;b<SIZE-2;b++) 
            Image[h][b] = tmpImage[h][b];
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
  
	ReadImage("lena.pgm",Image);
	t1 = getTime();
	for (i=0; i<COUNT; i++)
	   Filter(Image);
	t2 = getTime();
	WriteImage(Image, "lena_out.pgm");
    printf("result saved in lena_out.pgm\n");
    printf("Elapsed time: %f ms\n",(t2-t1)/COUNT);
}

