#include <mpi.h>

#include <iostream>

std::string prefix = "[all-scatter] ";

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int size, rank, sbufsize = 1024;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  char *rbuf = (char *)malloc(sizeof(char) * sbufsize * size);
  char *sbuf = (char *)malloc(sizeof(char) * sbufsize * size);
  for (int i = 0; i < size; ++i) {
    sbuf[i * sbufsize] = 'a' + rank;
    rbuf[i * sbufsize] = 'X';
  }

  MPI_Request *r = (MPI_Request *)malloc(sizeof(MPI_Request) * size * 2);
  double t = MPI_Wtime();
  for (int i = 0; i < size; ++i) {
    int dst = (rank + i) % size;
    MPI_Isend(&sbuf[i * sbufsize], sbufsize, MPI_CHAR, dst, 0, MPI_COMM_WORLD, &r[i * 2]);
    MPI_Irecv(&rbuf[dst * sbufsize], sbufsize, MPI_CHAR, dst, 0, MPI_COMM_WORLD, &r[i * 2 + 1]);
    std::cout << prefix << "rank " << rank << ": done with rank " << dst << '\n';
  }
  MPI_Waitall(size * 2, r, MPI_STATUSES_IGNORE);
  t = MPI_Wtime() - t;

  std::cout << prefix << "rank " << rank << ": sbuf: ";
  for (int i = 0; i < size; ++i)
    std::cout << sbuf[i * sbufsize];
  std::cout << '\n' << prefix << "rank " << rank << ": rbuf: ";
  for (int i = 0; i < size; ++i)
    std::cout << rbuf[i * sbufsize];
  std::cout << '\n' << prefix << "rank " << rank << ": time: " << t << '\n';

  free(sbuf);
  free(rbuf);
  free(r);
  MPI_Finalize();
  return 0;
}