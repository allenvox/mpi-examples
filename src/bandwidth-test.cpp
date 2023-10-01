#include <mpi.h>

#include <iostream>

std::string prefix = "[bandwidth] ";

int main(int argc, char **argv) {
  int rank, size, count = 32 * (1 << 20), iterations = 1000;
  char *buf = (char *)malloc(sizeof(char) * count);
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  double t = MPI_Wtime();
  for (int i = 0; i < iterations; i++) {
    if (rank == 0) {
      buf[0] = 1;
      MPI_Send(buf, count, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
      MPI_Recv(buf, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  t = (MPI_Wtime() - t) / iterations;
  std::cout << prefix << rank << ": buf = " << buf[0] << ", took " << t
            << " seconds" << '\n';
  double gbps = count / 1024.0 / 1024.0 / 1024.0 / t;
  std::cout << prefix << rank << "'s result: " << gbps << " GBps\n";

  free(buf);
  MPI_Finalize();
  return 0;
}