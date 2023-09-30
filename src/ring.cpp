#include <mpi.h>

#include <iostream>

std::string prefix = "[ring] ";

int main(int argc, char **argv) {
  int rank, size, count = 1024 * 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    std::cout << prefix << "info: " << size << " processes, size = " << count << " B\n";
  }
  int sendTo = (rank + 1) % size, receiveFrom = (rank - 1 + size) % size;
  char *mainbuf = (char*)malloc(sizeof(char) * size);

  double t = MPI_Wtime();
  for (int i = 0; i < size - 1; ++i) {
    std::cout << prefix << rank << ": sending to " << sendTo << ", receiving from " << receiveFrom << '\n';
    MPI_Sendrecv(&mainbuf[sendTo], 1, MPI_CHAR, sendTo, 0,
                 &mainbuf[receiveFrom], 1, MPI_CHAR, receiveFrom, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sendTo = receiveFrom;
    receiveFrom = (receiveFrom - 1 + size) % size;
  }
  t = MPI_Wtime() - t;
  std::cout << prefix << rank << ": elapsed " << t << " sec\n";
  for (int i = 0; i < size; ++i) {
    std::cout << prefix << rank << " contains " << mainbuf;
  }

  free(mainbuf);
  MPI_Finalize();
  return 0;
}