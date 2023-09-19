#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int rank, commsize, len, tag = 1;
  char host[MPI_MAX_PROCESSOR_NAME], msg[128];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Get_processor_name(host, &len);
  if (rank > 0) {
    snprintf(msg, NELEMS(msg), "Hello, master. I am %d of %d on %s", rank,
             commsize, host);
    MPI_Send(msg, NELEMS(msg), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    printf("Hello, World. I am master (%d of %d) on %s\n", rank, commsize,
           host);
    for (int i = 1; i < commsize; i++) {
      MPI_Recv(msg, NELEMS(msg), MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
               &status);
      printf("Message from %d: '%s'\n", status.MPI_SOURCE, msg);
    }
  }
  MPI_Finalize();
  return 0;
}