#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int rank, size, root = 0, count = 1024 * 1024;
  char *buf = malloc(sizeof(char) * count);
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double t = MPI_Wtime();
  if (rank == root) {
    printf("[broadcast] info: %d processes, root = %d, size = %d B\n", size, root, count);
    buf[0] = 'x';
    for (int i = 0; i < size; i++) {
      if (i != root) {
        MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        printf("[broadcast] %d: sent buf \"%s\" to %d\n", rank, buf, i);
      }
    }
  } else {
    MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buf[0] != 'x')
      fprintf(stderr, "[broadcast] %d: invalid buf\n", rank);
    else
      printf("[broadcast] %d: received buf \"%s\" from %d\n", rank, buf, root);
  }
  t = MPI_Wtime() - t;
  printf("[broadcast] %d: elapsed %.6f sec\n", rank, t);

  free(buf);
  MPI_Finalize();
  return 0;
}