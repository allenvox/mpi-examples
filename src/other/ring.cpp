#include <mpi.h>

#include <iostream>

std::string prefix = "[ring] ";

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int rank, size, bufsize = 1;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  char rank_char = 'a' + rank;
  char *buf = (char *)malloc(sizeof(char) * bufsize);
  buf[0] = rank_char;

  int next = (rank + 1) % size, prev = (rank - 1 + size) % size;
  std::cout << prefix << rank << ": send to " << next << ", receive from "
            << prev << '\n';

  double t = MPI_Wtime();
  MPI_Send(buf, bufsize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  while (1) {
    MPI_Recv(buf, bufsize, MPI_CHAR, prev, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    if (buf[0] == rank_char) {
      std::cout << prefix << rank << ": done, received rank_char '" << buf[0]
                << "'\n";
      break;
    }
    MPI_Send(buf, bufsize, MPI_CHAR, next, 0, MPI_COMM_WORLD);
  }
  t = MPI_Wtime() - t;
  std::cout << prefix << rank << ": elapsed " << t << " sec\n";

  free(buf);
  MPI_Finalize();
  return 0;
}