#include <pthread.h>
#include <stdlib.h>

# define N 100

double x[N];
int tid[N-1];
pthread_mutex_t mutex[N];
pthread_mutex_t mSwap;
pthread_barrier_t barrier;

int globalSwap = 1;
int run = 1;

void threadSwap(void* args)
{
  int index = (int)(*args);
  int swap = 0;
  double help;
  while(run)
  {
    swap = 0;
    pthread_mutex_lock (&mutex[index]);
    pthread_mutex_lock (&mutex[index + 1]);
    if(x[index] > x[index + 1])
    {
      swap++;
      help = x[index];
      x[index] = x[index + 1];
      x[index + 1] = help;
    }
    pthread_mutex_unlock (&mutex[index]);
    pthread_mutex_unlock (&mutex[index + 1]);
    //pthread_yield();
    pthread_mutex_lock(&mSwap);
    globalSwap |= swap;
    pthread_mutex_unlock(&mSwap);
    pthread_barrier_wait(&barrier);
    
    if (index == 1) {
      pthread_mutex_lock(&mSwap);
      if (globalSwap == 0) {
        run = 0;
      }
      globalSwap = 0;
      pthread_mutex_unlock(&mSwap);
    }
    pthread_barrier_wait(&barrier);
  }
}

void simpleSort()
{
  int i;
  for(i = 0; i < N-1; i++)
  {
    pthread_create(&tid[i], NULL, (void *)threadSwap,(void*) &i);
  }
  
  for(i = 0; i < N-1; i++)
  {
    pthread_join(tid[i]);
  }
  pthread_barrier_destroy(&barrier);
}

void main()
{
  int i;

  for(i = 0; i < N; i++)
  {
    x[i] = drand48();
    pthread_mutex_init(&mutex[i],NULL);
  }
  pthread_barrier_init(&barrier, NULL, N-1);
  simpleSort();
}
