//Bildpunkte sind unabhängig -> Datendekomposition der Punkte auf die Prozesse
//Zeilenweise Datendekomposition (blockweise), Problem: ungleiche Verteilung
//pixelweise zyklisch, Problem: aufwändigeres gathern(?)

#include <stdio.h>
#include <mpi.h>

#define XSIZE 2048
#define YSIZE 2048
#define WHOLE_SIZE XSIZE*YSIZE
#define XSTEP 3.0/XSIZE
#define YSTEP 3.0/YSIZE
#define XSTART -2.25
#define YSTART -1.5
//#define PROCESS_COUNT 8

int image[WHOLE_SIZE];
FILE *fd;

typedef struct cmplx {
  double re;
  double im;
}CMPLX;

CMPLX Square(CMPLX a) {
  CMPLX c;
  c.re=a.re*a.re-a.im*a.im;
  c.im=2*a.re*a.im;
  return(c);
}

CMPLX Add(CMPLX a,CMPLX b) {
  CMPLX c;
  c.re=a.re+b.re;
  c.im=a.im+b.im;
  return(c);
}

float SquareAbs(CMPLX a) {
  return(a.re*a.re+a.im*a.im);
}

void merge(int offset, int *buffer) {
    for(int i = 0; i < WHOLE_SIZE/PROCESS_COUNT; i++) {
      image[i*PROCESS_COUNT + offset] = buffer[i];
    }
}

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  int i, j, it;
  int index = 0;
  CMPLX z,c;
  int processID;
  int localBuffer[WHOLE_SIZE/PROCESS_COUNT];

  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  for(i = 0; i < YSIZE; i++) {
    for(j = processID; j < XSIZE; j += PROCESS_COUNT) {
      z.re = 0.0;
      z.im = 0.0;
      c.re = XSTART + j*XSTEP;
      c.im = YSTART + i*YSTEP;
      it = 0;

      while ((SquareAbs(z)<100.0) && (it<256)) {
      	z=Square(z);
      	z=Add(z,c);
      	it++;
      }

      localBuffer[index]= it-1;
      index++;
    }
  }

  if(processID > 0) {
    MPI_Send(localBuffer, WHOLE_SIZE/PROCESS_COUNT, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    MPI_Status status;

    merge(0, localBuffer);

    for(int process = 1; process < PROCESS_COUNT; process++) {
      MPI_Recv(localBuffer, WHOLE_SIZE/PROCESS_COUNT, MPI_INT, process, 0, MPI_COMM_WORLD, &status);
      merge(process, localBuffer);
    }

    fd=fopen("mandel.pgm","w");
    fprintf(fd, "P5 ");
    fprintf(fd, "%d %d ", YSIZE, XSIZE);
    fprintf(fd, "%d ", 255);   /* Max. Grauwert */

    for(i=0;i<YSIZE;i++) {
      for(j=0;j<XSIZE;j++)
        fprintf(fd,"%c",(unsigned char) image[i*XSIZE+j]);
    }
    fclose(fd);
  }
  MPI_Finalize();
}
