#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int rank, size, tag = 1, count = 128;
  char *buf = malloc(sizeof(char) * count);
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank > 0) {
    snprintf(buf, count, "[gather] %d is here", rank);
    MPI_Send(buf, count, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    printf("[gather] %d (master) is here\n", rank);
    for (int i = 1; i < size; i++) {
      MPI_Recv(buf, count, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
               &status);
      printf("[gather] %d: received message from %d: '%s'\n", rank, status.MPI_SOURCE, buf);
    }
  }

  free(buf);
  MPI_Finalize();
  return 0;
}