#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size, len;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Get_processor_name(procname, &len);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  printf("Rank %d on %s\n", rank, procname);

  int count = 32 * (1 << 20);
  char *buf = malloc(sizeof(*buf) * count);
  double t = MPI_Wtime();

  int iterations = 1000;
  for (int i = 0; i < iterations; i++) {
    if (rank == 0) {
      buf[0] = 1;
      MPI_Send(buf, count, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
      MPI_Recv(buf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  t = (MPI_Wtime() - t) / iterations;
  printf("Rank %d/%d: buf = %d\n", rank, size, buf[0]);
  printf("Rank %d/%d: time (sec): %.6f\n", rank, size, t);

  double gbps = count / 1024.0 / 1024.0 / 1024.0 / t;
  printf("Average bandwidth: %.2f GBps\n", gbps);

  free(buf);
  MPI_Finalize();
  return 0;
}