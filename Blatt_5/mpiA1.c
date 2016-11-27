#include <stdio.h>
#include <mpi.h>
#define COMM1 1
#define COMM2 2
int main(int argc, char **argv) {
  int num_procs, my_id;
  double time = 0;
  double recTime = 0;
  double intermediate = 0;
  double recIntermediate;
  double res[N];
  double result = 0;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  /* find out MY process ID, and how many processes were started. */

  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  recTime = 0;
  for (int count = 0; count < N; count++) {
    if (my_id == 0) {
      time = MPI_Wtime();
      printf("Process %i is sending: '%lf'\n", my_id, time);
      MPI_Send((void *)&time, 1, MPI_DOUBLE, 1, COMM1, MPI_COMM_WORLD);
      MPI_Recv((void *)&recIntermediate, 1, MPI_DOUBLE, 1, COMM2, MPI_COMM_WORLD, &status);
      res[count] = recIntermediate - MPI_Wtime();
    }else if(my_id == 1){
      MPI_Recv((void *)&recTime, 1, MPI_DOUBLE, 0, COMM1, MPI_COMM_WORLD, &status);
      intermediate = MPI_Wtime() + (MPI_Wtime() - recTime);
      printf("Process %i is answering: '%lf'\n", my_id, intermediate);
      MPI_Send((void *)&intermediate, 1, MPI_DOUBLE, 0, COMM2, MPI_COMM_WORLD);
    }
  }
  if (my_id == 1) {
    for (int i = 0; i < N; ++i) {
      result += res[i];
    }
    printf("Time: %lf\n", result/N);
  }
  MPI_Finalize();
}
