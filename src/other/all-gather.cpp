#include <mpi.h>

#include <iostream>

std::string prefix = "[all-gather] ";

int main(int argc, char **argv) {
  int rank, p, bufsize = 1024 * 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  char *sbuf = (char *)malloc(sizeof(char) * bufsize);
  int rbufsize = sizeof(char) * bufsize * p;
  char *rbuf = (char *)malloc(rbufsize);

  sbuf[0] = 'a' + rank;
  std::cout << prefix << rank << ": sbuf[0] = " << sbuf[0] << '\n';
  rbuf[rank * bufsize] = *sbuf;

  double t = MPI_Wtime();
  for (int i = 0; i < p - 1; i++) {
    int sendTo = (rank + i + 1) % p;
    int receiveFrom = (rank - i - 1 + p) % p;
    // std::cout << prefix << rank << ": send to " << sendTo
    //           << ", receive from" << receiveFrom << '\n';
    MPI_Sendrecv(sbuf, 1, MPI_CHAR, sendTo, 0, &rbuf[receiveFrom * bufsize], 1,
                 MPI_CHAR, receiveFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // std::cout << prefix << rank << ": rbuf[" << receiveFrom * bufsize
    //           << "] = " << rbuf[receiveFrom * bufsize] << '\n';
  }
  t = MPI_Wtime() - t;

  std::cout << prefix << rank << ": rbuf = ";
  for (int i = 0; i < rbufsize; i++) {
    if (rbuf[i] != 0) std::cout << rbuf[i];
  }
  std::cout << ", took " << t << " seconds" << '\n';

  free(sbuf);
  free(rbuf);
  MPI_Finalize();
  return 0;
}