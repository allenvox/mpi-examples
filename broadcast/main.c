#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int count = 8 * 1024 * 1024;
  char *buf = malloc(sizeof(int) * count);

  int root = 0;
  if (rank == root) {
    buf[0] = 'x';
    for (int i = 0; i < size; i++) {
      if (i != root) {
        MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        printf("[broadcast] Sent buf \"%s\" from %d to %d\n", buf, root, i);
      }
    }
  } else {
    MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buf[0] != 'x')
      fprintf(stderr, "[broadcast] %d invalid buf\n", rank);
    else
      printf("[broadcast] Received buf \"%s\" from %d\n", buf, root);
  }

  free(buf);
  MPI_Finalize();
  return 0;
}