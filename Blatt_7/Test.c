#include<stdio.h>
#include<unistd.h>

int main()
{
  char host[80], host2[80];

  #pragma omp target data device(0) map(from:host)
  {
    #pragma omp target device(0)
    {
      gethostname(host, 80);
    }
  }

  printf("Hello from %s\n", host);
  gethostname(host2, 80);
  printf("Hello from %s\n",host2);  
}
