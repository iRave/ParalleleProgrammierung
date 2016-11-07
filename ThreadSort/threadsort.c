#include <pthread.h>
#include <stdlib.h>

# define N 100

double x[N];

void threadSwap(void* args)
{
  int index = (int)(*args);
  int help;
  while(true)
  {
    if(x[index] > x[index + 1])
    {
      help = x[index];
      x[index] = x[index + 1];
      x[index + 1] = help;
    }
  }
}

void simpleSort()
{
  for(i = 0; i < N-1; i++)
  {
    pthread_create(&tid[i], NULL, (void *)threadSwap,(void*) &i);
  }
}

void main()
{
  int i;
  int tid[N-1];

  for(i = 0; i < N; i++)
  {
    x[i] = drand48();
  }
}
