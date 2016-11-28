#include <stdio.h>
#include <mpi.h>
#define COMM1 1
#define COMM2 2
int main(int argc, char **argv) {
  int my_id;
  double time = 0;
  double recNumber = 0;
  double res[N];
  double result = 0;
  MPI_Status status;
  MPI_Init(&argc, &argv);

  /* find out MY process ID, and how many processes were started. */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  for (int count = 0; count < N; count++) {
    if (my_id == 0) {
      time = MPI_Wtime();
      printf("Process %i is sending: '%lf'\n", my_id, count);
      MPI_Send((void *)&count, 1, MPI_INT, 1, COMM1, MPI_COMM_WORLD);
      MPI_Recv((void *)&recNumber, 1, MPI_INT, 1, COMM2, MPI_COMM_WORLD, &status);
      res[count] = MPI_Wtime() - time;
    }else if(my_id == 1){
      MPI_Recv((void *)&recNumber, 1, MPI_INT, 0, COMM1, MPI_COMM_WORLD, &status);
      printf("Process %i is answering: '%lf'\n", my_id, recNumber);
      MPI_Send((void *)&recNumber, 1, MPI_INT, 0, COMM2, MPI_COMM_WORLD);
    }
  }
  if (my_id == 0) {
    for (int i = 0; i < N; ++i) {
      result += res[i];
    }
    printf("Time: %lf\n", result/N);
  }
  MPI_Finalize();
}
