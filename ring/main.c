#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int receiver = (rank + 1) % size, sender = (rank - 1 + size) % size;
  int *mainbuf = malloc(sizeof(int) * size);
  for (int i = 0; i < size; ++i) {
    mainbuf[i] = -1;
  }
  mainbuf[rank] = rank;
  int r = sender, s = rank;

  double t = MPI_Wtime();
  for (int i = 0; i < size - 1; ++i) {
    printf("[ring] %d: sending to %d, receiving from %d\n", rank,
           receiver, sender;
    MPI_Sendrecv(&mainbuf[s], 1, MPI_INT, receiver, 0, &mainbuf[r], 1,
                 MPI_INT, sender, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    s = r;
    r = (r - 1 + size) % size;
  }
  t = MPI_Wtime() - t;

  for (int i = 0; i < size; ++i) {
    if (!(i)) {
      printf("[ring] %d: contain [%d, ", rank, mainbuf[i]);
    } else {
      if (i == size - 1) {
        printf("%d]\n", mainbuf[i]);
      } else {
        printf("%d, ", mainbuf[i]);
      }
    }
  }
  printf("[ring] %d: elapsed %.6f sec\n", rank, t);

  free(mainbuf);
  MPI_Finalize();
  return 0;
}