#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
int main(int argc, char **argv) {
  int num_procs, my_id;
  double time = 0;
  double recTime = 0;
  double res[N];
  double result = 0;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  /* find out MY process ID, and how many processes
  were started. */

  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  recTime = 0;
  for (int count = 0; count < N; count++) {
    if (my_id == 0) {
      time = MPI_Wtime();
      //printf("Process %i is sending: '%lf'\n", my_id, time);
      MPI_Send((void *)&time, 1, MPI_DOUBLE, 1, count, MPI_COMM_WORLD);
      //usleep(10);
    }else if(my_id == 1){
      MPI_Recv((void *)&recTime, 1, MPI_DOUBLE, 0, count, MPI_COMM_WORLD, &status);
      //printf("Process %i received: '%lf'\n", my_id, recTime);
      //printf("Transfere time: %lf\n", (MPI_Wtime() - recTime));
      res[count] = MPI_Wtime() - recTime;
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
