#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size, count = 1024 * 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if(rank == 0) {
    printf("[ring] info: %d processes, size = %d B\n", size, count);
  }
  int sendTo = (rank + 1) % size, receiveFrom = (rank - 1 + size) % size;
  char *mainbuf = malloc(sizeof(char) * size);

  double t = MPI_Wtime();
  for (int i = 0; i < size - 1; ++i) {
    printf("[ring] %d: sending to %d, receiving from %d\n", rank,
           sendTo, receiveFrom);
    MPI_Sendrecv(&mainbuf[sendTo], 1, MPI_CHAR, sendTo, 0, &mainbuf[receiveFrom], 1,
                 MPI_CHAR, receiveFrom, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    sendTo = receiveFrom;
    receiveFrom = (receiveFrom - 1 + size) % size;
  }
  t = MPI_Wtime() - t;
  printf("[ring] %d: elapsed %.6f sec\n", rank, t);

  for (int i = 0; i < size; ++i) {
    if (!(i)) {
      printf("[ring] %d contains [%d, ", rank, mainbuf[i]);
    } else {
      if (i == size - 1) {
        printf("%d]\n", mainbuf[i]);
      } else {
        printf("%d, ", mainbuf[i]);
      }
    }
  }

  free(mainbuf);
  MPI_Finalize();
  return 0;
}