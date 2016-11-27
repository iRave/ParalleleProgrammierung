#include <stdio.h>

#define XSIZE 2048
#define YSIZE 2048
#define XSTEP 3.0/XSIZE
#define YSTEP 3.0/YSIZE
#define XSTART -2.25
#define YSTART -1.5

int image[XSIZE*YSIZE];
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

main(){
  int i,j,it;
  CMPLX z,c;
  
  for(i=0;i<YSIZE;i++) {
    for(j=0;j<XSIZE;j++) {
      z.re=0.0;
      z.im=0.0;
      c.re=XSTART+j*XSTEP;
      c.im=YSTART+i*YSTEP;
      it=0;
      while ((SquareAbs(z)<100.0) && (it<256)) {
	z=Square(z);
	z=Add(z,c);
	it++;
      }
      image[i*XSIZE+j]= it-1;
    }
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
