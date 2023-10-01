#include <mpi.h>

#include <iostream>

std::string prefix = "[gather] ";

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int rank, size, sbufsize = 1024;
  char *buf;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    std::cout << prefix + "info: " << size
              << " processes, root = 0, sbufsize = " << sbufsize << " B\n";
  }

  double t = MPI_Wtime();
  if (rank > 0) {  // not root, buf = sbuf
    buf = (char *)malloc(sizeof(char) * sbufsize);
    buf[0] = 'a' + rank - 1;
    MPI_Send(buf, sbufsize, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
  } else {  // root, buf = rbuf
    buf = (char *)malloc(sizeof(char) * sbufsize * (size - 1));
    MPI_Status status;
    std::cout << prefix << rank << " (master) is here\n";
    for (int i = 1; i < size; i++) {
      MPI_Recv(&buf[(rank - 1) * sbufsize], sbufsize, MPI_CHAR, MPI_ANY_SOURCE,
               1, MPI_COMM_WORLD, &status);
      std::cout << prefix << rank << ": received message from "
                << status.MPI_SOURCE << ": \"" << buf[(rank - 1) * sbufsize]
                << "\"\n";
    }
  }
  t = MPI_Wtime() - t;
  std::cout << prefix << rank << ": elapsed " << t << " sec\n";

  free(buf);
  MPI_Finalize();
  return 0;
}