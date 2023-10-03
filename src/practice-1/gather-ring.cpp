#include <mpi.h>

#include <iostream>

std::string prefix = "[gather-ring] ";

int main(int argc, char **argv) {
  int rank, p, sbufsize = 1024 * 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if(rank == 0) {
    std::cout << prefix << "info: " << p << " processes, message size = " << sbufsize << " B\n";
  }

  char *rbuf = (char *)malloc(sizeof(char) * sbufsize * p);
  rbuf[rank * sbufsize] = 'a' + rank;
  std::cout << prefix << rank << ": sbuf = " << rbuf[rank * sbufsize] << '\n';

  int sendTo = (rank + 1) % p;
  int receiveFrom = (rank - 1 + p) % p;
  std::cout << prefix << rank << ": send to " << sendTo << ", receive from " << receiveFrom << '\n';

  double t = MPI_Wtime();
  for (int i = 0; i < p - 1; i++) {
    int curBufOwner = (rank - i - 1 + p) % p;
    int prevBufOwner = (curBufOwner + 1) % p;
    std::cout << prefix << rank << ": cur buf owner is " << curBufOwner << '\n';
    MPI_Sendrecv(&rbuf[prevBufOwner * sbufsize], sbufsize, MPI_CHAR, sendTo, 0, &rbuf[curBufOwner * sbufsize], sbufsize, MPI_CHAR, receiveFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << prefix << rank << ": received to rbuf = " << rbuf[curBufOwner * sbufsize] << '\n';
  }
  t = MPI_Wtime() - t;

  std::cout << prefix << rank << ": rbuf = ";
  for (int i = 0; i < sbufsize * p; i += sbufsize) {
    std::cout << rbuf[i];
  }
  std::cout << ", took " << t << " seconds" << '\n';

  free(rbuf);
  MPI_Finalize();
  return 0;
}