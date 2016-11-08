#include <omp>
#include <stdio>
int main(int argc, char* argv[])
{
  printf("I am the main thread.\n");
  #pragma omp parallel
  {
    printf("I am thread %d of %d.\n", omp_get_thread_num(), omp_get_num_threads());
  }
  return 0;
}
