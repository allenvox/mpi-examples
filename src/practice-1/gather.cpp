#include <mpi.h>

#include <iostream>

std::string prefix = "[gather] ";

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);
  int rank, size, sbufsize = 1024, root = 0;
  char *buf;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    std::cout << prefix + "info: " << size << " processes, root = " << root
              << ", sbufsize = " << sbufsize << " B\n";
  }

  double t = MPI_Wtime();
  if (rank != root) {  // buf = sbuf
    buf = (char *)malloc(sizeof(char) * sbufsize);
    buf[0] = 'a' + rank - 1;
    MPI_Send(buf, sbufsize, MPI_CHAR, root, 0, MPI_COMM_WORLD);
    std::cout << prefix << rank << ": sent sbuf '" << buf[0] << "' to root\n";

  } else {  // buf = rbuf
    buf = (char *)malloc(sizeof(char) * sbufsize * size);
    char *local = (char *)malloc(sizeof(char) * sbufsize);
    MPI_Status status;
    MPI_Recv(local, sbufsize, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
             &status);
    int source = status.MPI_SOURCE;
    std::copy(&local[0], &local[sbufsize - 1], &buf[(source - 1) * sbufsize]);
    std::cout << prefix << rank << ": received message from " << source << ": '"
              << buf[(source - 1) * sbufsize] << "'\n";
  }
  t = MPI_Wtime() - t;

  if (rank == root) {
    std::cout << prefix << rank << ": rbuf = \"";
    for (int i = 0; i < sbufsize * (size - 1); i += sbufsize) {
      std::cout << buf[i];
    }
    std::cout << "\"\n";
  }
  std::cout << prefix << rank << ": elapsed " << t << " seconds\n";

  free(buf);
  MPI_Finalize();
  return 0;
}